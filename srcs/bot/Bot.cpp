/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:28:02 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/11 14:25:19 by ben              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// ============ Constructor ============

Bot::Bot() : sockfd(-1), connected(false) {
	initCommands();
}

Bot::Bot(const Bot &src) {
    *this = src;
	initCommands();
}

// ============ Destructor ============

Bot::~Bot() {
	if (connected) {
		disconnect();
	}
}

// ============ Operator ============

Bot &Bot::operator=(const Bot &rhs) {
	if (this != &rhs) {
        this->sockfd = rhs.sockfd;
        this->connected = rhs.connected;
        this->address = rhs.address;
        this->port = rhs.port;
        this->password = rhs.password;
		initCommands();
    }
    return (*this);
}

// ============ Member Functions ============

void Bot::initCommands() {
    commands["!help"] = &Bot::help;
}

std::string Bot::help() {
    return "Available commands: !help";
}

bool Bot::is_cmd(const std::string &msg, std::string &command) {
    size_t pos = msg.find("PRIVMSG");
    if (pos != std::string::npos) {
        size_t cmd_start = msg.find(":", pos);
        if (cmd_start != std::string::npos) {
            command = msg.substr(cmd_start + 1);
            return true;
        }
    }
    return false;
}

void Bot::apply_cmd(const std::string &command) {
	std::string cmd = command;
    std::cout << "cmd list \n";

    for (std::map<std::string, CommandFunction>::iterator it = commands.begin(); it != commands.end(); ++it) {
        std::cout << "[" << it->first << "]" << std::endl;
    }

    std::cout << "cmd received : [" << cmd << "]" << std::endl;

	cmd.erase(std::remove(cmd.begin(), cmd.end(), '\r'), cmd.end());

    std::cout << "clean cmd received : [" << cmd << "]" << std::endl;

    std::map<std::string, CommandFunction>::iterator it = commands.find(cmd);
    if (it != commands.end()) {
        CommandFunction func = it->second;
        std::string response = (this->*func)();
        std::string reply = "PRIVMSG " + std::string("bberkrou") + " :" + response + "\r\n";
        std::cout << "DEBUG : " << reply << "\n";
        send(sockfd, reply.c_str(), reply.length(), 0);
    } else {
        std::string reply = "PRIVMSG " + std::string("bberkrou") + " :Command not found\r\n";
        send(sockfd, reply.c_str(), reply.length(), 0);
    }
}

// ===================================================================================================
// ========================================= Connection PART =========================================
// ===================================================================================================

/**
 * @brief Connects the bot to the server using the provided address, port, and password.
 * 
 * Initializes the connection process by creating a socket, setting up the server address structure,
 * connecting to the server, sending authentication credentials, and handling the server's response.
 * 
 * @param address The server address as a string.
 * @param port The server port as a string.
 * @param password The password for server authentication.
 * 
 * @throws std::runtime_error If any step in the connection process fails.
 */
void Bot::connect(const std::string &address, const std::string &port, const std::string &password) {
    this->address = address;
    this->port = port;
    this->password = password;

    struct sockaddr_in serv_addr;

    createSocket();
    setupAddressStruct(address, port, serv_addr);
    connectToServer(serv_addr);
    sendCredentials();
    handleServerResponse();
}

/**
 * @brief Creates a socket for connecting to the server.
 * 
 * Initializes a socket using IPv4 and TCP. Throws an exception if the socket creation fails.
 * 
 * @throws std::runtime_error If socket creation fails.
 */
void Bot::createSocket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Socket creation error");
}

/**
 * @brief Sets up the server address structure.
 * 
 * Configures the sockaddr_in structure with the provided address and port. 
 * Converts the address to binary form and sets the port. Throws an exception if the address is invalid.
 * 
 * @param address The server address as a string.
 * @param port The server port as a string.
 * @param serv_addr The sockaddr_in structure to be configured.
 * 
 * @throws std::runtime_error If the address is invalid or not supported.
 */
void Bot::setupAddressStruct(const std::string &address, const std::string &port, struct sockaddr_in &serv_addr) {
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(port));

    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address/ Address not supported");
}

/**
 * @brief Connects to the server using the given address structure.
 * 
 * Attempts to establish a connection to the server with the provided 
 * sockaddr_in structure. Throws an exception if the connection fails.
 * 
 * @param serv_addr The server address structure.
 * 
 * @throws std::runtime_error If the connection fails.
 */
void Bot::connectToServer(struct sockaddr_in &serv_addr) {
    if (::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection Failed");
}

/**
 * @brief Sends authentication commands (PASS, NICK, USER) to the server.
 * 
 * Constructs and sends the PASS, NICK, and USER commands to the server
 * using the provided password and predefined nick and user values.
 */
void Bot::sendCredentials() {
    std::string pass_cmd = "PASS " + this->password + "\r\n";
    send(sockfd, pass_cmd.c_str(), pass_cmd.length(), 0);

    std::string nick_cmd = "NICK botnick\r\n";
    send(sockfd, nick_cmd.c_str(), nick_cmd.length(), 0);

    std::string user_cmd = "USER botnick 0 * :Bot User\r\n";
    send(sockfd, user_cmd.c_str(), user_cmd.length(), 0);
}

/**
 * @brief Handles the server's response after sending credentials.
 * 
 * Waits for and reads the server's response. Checks for specific messages 
 * indicating either a successful connection ("001") or an incorrect password ("464").
 * Disconnects on error.
 */
void Bot::handleServerResponse() {
    char buffer[1024] = {0};
    std::cout << "Waiting for server response" << std::endl;
    int valread = read(sockfd, buffer, 1024);
    std::cout << "Server response received" << std::endl;
    if (valread > 0) {
        buffer[valread] = '\0';
        std::cout << "Server: " << buffer << std::endl;
        if (std::string(buffer).find("464") != std::string::npos) {
            std::cerr << "Error: Password incorrect" << std::endl;
            disconnect();
            return;
        } else if (std::string(buffer).find("001") != std::string::npos) {
            std::cout << "Successfully connected to the server" << std::endl;
            connected = true;
            return;
        }
    }
    std::cerr << "Error: Unable to connect to the server" << std::endl;
    disconnect();
}

void Bot::disconnect() {
	if (connected) {
		close(sockfd);
		connected = false;
		std::cout << "Disconnected from the server" << std::endl;
	}
}

// ===================================================================================================
// ======================================= Handle message PART =======================================
// ===================================================================================================

/**
 * @brief Runs the bot's main loop for handling server responses.
 * 
 * Enters a loop to continuously wait for and handle server responses until disconnected.
 */
void Bot::run() {
    char buffer[4096] = {0};

    while (connected) {
        if (waitForServerResponse()) {
            int valread = readServerResponse(buffer, sizeof(buffer));
            handleServerResponse(buffer, valread);
        } else {
            std::cerr << "Error: Timeout waiting for server response" << std::endl;
            disconnect();
        }
    }
}

/**
 * @brief Waits for server response using poll.
 * 
 * Polls the socket for incoming data for a specified timeout period.
 * 
 * @param timeout_ms The timeout period in milliseconds.
 * @return True if data is available to read within the timeout period, false otherwise.
 */
bool Bot::waitForServerResponse() {
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    while (true) {
        int poll_ret = poll(&pfd, 1, -1); // Attente indéfinie jusqu'à ce que des données soient disponibles
        if (poll_ret > 0) {
            return true; // Des données sont disponibles à lire
        } else if (poll_ret < 0) {
            // Erreur de poll, gérer l'erreur ici si nécessaire
            std::cerr << "Error: poll() failed" << std::endl;
            return false;
        }
        // Aucune donnée disponible, continuer à attendre
    }
}

/**
 * @brief Reads server response from the socket.
 * 
 * Reads data from the socket into the provided buffer.
 * 
 * @param buffer The buffer to store the server response.
 * @param size The size of the buffer.
 * @return The number of bytes read, or -1 if an error occurs.
 */
int Bot::readServerResponse(char *buffer, int size) {
    return read(sockfd, buffer, size);
}

/**
 * @brief Handles the received server response.
 * 
 * Parses the server response and performs appropriate actions based on the response.
 * 
 * @param buffer The buffer containing the server response.
 * @param size The size of the buffer.
 */
void Bot::handleServerResponse(char *buffer, int size) {
    if (size > 0) {
        buffer[size] = '\0';
        std::cout << "Server response received" << std::endl;
        std::cout << "============ MSG Recus ============ " << std::endl;
        std::cout << GREEN << buffer << RESET << std::endl;
        std::cout << "===================================" << std::endl;

        std::string msg(buffer);
        std::string command;
        if (is_cmd(msg, command)) {
            std::cout << "Command received: " << command << std::endl;
            apply_cmd(command);
        } else {
            std::cout << "No command received" << std::endl;
        }
    } else if (size == 0) {
        std::cerr << "Server disconnected" << std::endl;
        disconnect();
    } else {
        std::cerr << "Error: Unable to read from server" << std::endl;
        disconnect();
    }
}



// ============ Getters ============

// ============ Setters ============
