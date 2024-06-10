/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:28:02 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/10 20:14:01 by bberkrou         ###   ########.fr       */
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


void Bot::connect(const char *address, const char *port, const char *password) {
    this->address = address;
    this->port = port;
    this->password = password;

    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Socket creation error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));

    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address/ Address not supported");

    if (::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw std::runtime_error("Connection Failed");

    std::string pass_cmd = "PASS " + this->password + "\r\n";
    send(sockfd, pass_cmd.c_str(), pass_cmd.length(), 0);

    std::string nick_cmd = "NICK botnick\r\n";
    send(sockfd, nick_cmd.c_str(), nick_cmd.length(), 0);

    std::string user_cmd = "USER botnick 0 * :Bot User\r\n";
    send(sockfd, user_cmd.c_str(), user_cmd.length(), 0);

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

void Bot::run() {
	char buffer[4096] = {0};
	int valread;

	while (connected) {
		valread = read(sockfd, buffer, 1024);
		if (valread == 0) {
			std::cerr << "Server disconnected" << std::endl;
			disconnect();
			return;
		}
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
	}
}

void Bot::disconnect() {
	if (connected) {
		close(sockfd);
		connected = false;
		std::cout << "Disconnected from the server" << std::endl;
	}
}

// ============ Getters ============

// ============ Setters ============
