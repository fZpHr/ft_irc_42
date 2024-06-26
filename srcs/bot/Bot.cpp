/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:28:02 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/17 17:36:51 by cpeterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot/Bot.hpp"

// ===================================================================================================
// ======================================== Utils Class PART =========================================
// ===================================================================================================

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
        this->connected = false;
        this->address = rhs.address;
        this->port = rhs.port;
        this->password = rhs.password;
		initCommands();
    }
    return (*this);
}

// ===================================================================================================
// ========================================== Commands PART ==========================================
// ===================================================================================================

/**
 * @brief Starts a new Tic Tac Toe game for the user.
 * 
 * If the user does not already have an ongoing game, this function initializes a new game and 
 * provides the game board. If a game is already in progress, it informs the user.
 * 
 * @param username The username of the player.
 * @param args Unused parameter.
 * @return A string containing the game board or an appropriate message.
 */
std::string Bot::tictactoe(const std::string &username, const std::string &args) {
    (void)args;
    if (games.find(username) == games.end()) {
        games[username] = TicTacToe();
        return ("===== Game started =====\r\n" + games[username].getBoard() + "\r\n" + "\tMake your move {!play row-col}\r\n");
    }
    return ("===== Game already started =====\r\n\tMake your move {!play row-col}\r\n");
}

/**
 * @brief Makes a move in the ongoing Tic Tac Toe game.
 * 
 * This function allows the user to make a move by specifying the row and column. 
 * If no game is currently active, it prompts the user to start a new game.
 * 
 * @param username The username of the player.
 * @param args The move specified as row-col (e.g. "1-1").
 * @return A string containing the result of the move or an appropriate message.
 */
std::string Bot::play(const std::string &username, const std::string &args) {
    if (games.find(username) != games.end()) {
        std::string answer = games[username].play(args, 'X');
        return (answer);
    }
    return ("===== No game started, type !tictactoe to start a game ===== \r\n");
}

/**
 * @brief Restarts the current Tic Tac Toe game.
 * 
 * If a game is ongoing, this function resets the game board and allows the user to start over. 
 * If no game is active, it prompts the user to start a new game.
 * 
 * @param username The username of the player.
 * @param args Unused parameter.
 * @return A string containing the new game board or an appropriate message.
 */
std::string Bot::restart(const std::string &username, const std::string &args) {
    (void)args;
    if (games.find(username) != games.end()) {
        games[username].resetBoard();
        return ("===== Game restarted =====\r\n" + games[username].getBoard() + "\r\n" + "\tMake your move {!play row-col}\r\n");
    }
    return ("===== No game started, type !tictactoe to start a game =====\r\n");
}

// ===================================================================================================
// ======================================== Help Commands PART =======================================
// ===================================================================================================

/**
 * @brief Provides help information for commands.
 * 
 * Depending on the arguments provided, this function returns a general list of commands 
 * or specific information about a particular command.
 * 
 * @param username The username of the requester.
 * @param args The command for which help is requested.
 * @return A string containing the help information.
 */
std::string Bot::help(const std::string &username, const std::string &args) {

    if (args.empty())
        return (help_list(username, args));
    if (args == "tictactoe")
        return (help_tictactoe(username, args));
    if (args == "play")
        return (help_play(username, args));
    if (args == "restart")
        return (help_restart(username, args));
    else
        return ("No information for this cmd [" + args + "]\n");
}

/**
 * @brief Provides a list of all available commands.
 * 
 * This function generates a list of all commands registered in the bot, along with a brief 
 * description of each command. It also instructs users on how to get more information about 
 * a specific command.
 * 
 * @param username The username of the requester.
 * @param args Unused parameter.
 * @return A string containing the list of commands.
 */
std::string Bot::help_list(const std::string &username, const std::string &args) {
    (void)username;
    (void)args;
    std::string answer = "\n================ Cmd list ================\r\n";
    for (std::map<std::string, t_data_func>::iterator it = commands.begin(); it != commands.end(); ++it) {
        answer += "\t[" + it->first + "] => {" + it->second.description + "}\r\n";
    }
    answer += "==========================================\r\n";
    answer += "For more information about a command, type !help <command>\r\n";
    answer += "==========================================\r\n";
    return (answer);
}

/**
 * @brief Provides help information for the !tictactoe command.
 * 
 * This function explains the rules of Tic Tac Toe and instructs users on how to play the game 
 * using the bot.
 * 
 * @param username The username of the requester.
 * @param args Unused parameter.
 * @return A string containing the help information for the !tictactoe command.
 */
std::string Bot::help_tictactoe(const std::string &username, const std::string &args) {
    (void)args;
    (void)username;
    std::string answer = "=================== Help TicTacToe ===================\r\n";
    answer += "\tThe game is played on a 3x3 grid.\r\n";
    answer += "\tYou are X, IA is O. Players take turns putting their marks in empty squares.\r\n";
    answer += "\tThe first player to get 3 of her marks in a row (up, down, across, or diagonally) is the winner.\r\n";
    answer += "\tWhen all 9 squares are full, the game is over. If no player has 3 marks in a row, the game ends in a tie.\r\n";
    answer += "\tTo make a move, type !play row-col (e.g. !play 1-1)\r\n";
    answer += "\tTo restart the game, type !restart\r\n";
    return (answer);
}

/**
 * @brief Provides help information for the !play command.
 * 
 * This function instructs users on how to make a move in the Tic Tac Toe game using the bot.
 * 
 * @param username The username of the requester.
 * @param args Unused parameter.
 * @return A string containing the help information for the !play command.
 */
std::string Bot::help_play(const std::string &username, const std::string &args) {
    (void)args;
    (void)username;
    std::string answer = "=================== Help Play ===================\r\n";
    answer += "\tTo make a move, type !play row-col (e.g. !play 1-1)\r\n";
    return (answer);
}

/**
 * @brief Provides help information for the !restart command.
 * 
 * This function instructs users on how to restart the Tic Tac Toe game using the bot.
 * 
 * @param username The username of the requester.
 * @param args Unused parameter.
 * @return A string containing the help information for the !restart command.
 */
std::string Bot::help_restart(const std::string &username, const std::string &args) {
    (void)username;
    (void)args;
    std::string answer = "=================== Help Restart ===================\r\n";
    answer += "\tTo restart the game, type !restart\r\n";
    return (answer);
}

// ===================================================================================================
// ======================================= Utils Commands PART =======================================
// ===================================================================================================

/**
 * @brief Initializes the bot's commands.
 * 
 * This function registers all available commands that the bot can execute. Each command
 * is associated with a description and a corresponding member function that handles the command.
 */
void Bot::initCommands() {
    addCommand("!tictactoe", "Play at TicTacToe with a IA.", &Bot::tictactoe);
    addCommand("!play", "Make a move at TicTacToe", &Bot::play);
    addCommand("!restart", "Restart the game.", &Bot::restart);
    addCommand("!help", "Display all available commands.", &Bot::help);
}

/**
 * @brief Adds a command to the bot's command list.
 * 
 * This function adds a new command to the bot's internal command map. Each command
 * is stored with its name, description, and the member function that handles the command.
 * 
 * @param command_name The name of the command.
 * @param description A brief description of what the command does.
 * @param func The member function that handles the command.
 */
void Bot::addCommand(const std::string& command_name, const std::string& description, CommandFunction func) {
    t_data_func command_data;
    command_data.description = description;
    command_data.func = func;
    commands[command_name] = command_data;
}

/**
 * @brief Checks if a message is a command.
 * 
 * This function checks if a given message contains a command. If a command is found,
 * it extracts the command from the message.
 * 
 * @param msg The message string to be checked.
 * @param command A reference to a string where the extracted command will be stored.
 * @return true If the message contains a command.
 * @return false If the message does not contain a command.
 */
bool Bot::is_cmd(const std::string &msg, std::string &command) {
    size_t pos = msg.find("PRIVMSG");
    if (pos != std::string::npos) {
        size_t cmd_start = msg.find(":", pos);
        if (cmd_start != std::string::npos) {
            command = msg.substr(cmd_start + 1);
            return (true);
        }
    }
    return (false);
}

/**
 * @brief Sends a message to a user in multiple parts if necessary.
 * 
 * This function takes a message string and a username, splits the message by newline characters,
 * and sends each part as a separate message to the user. It constructs the message in the format
 * required by the IRC protocol and sends it through the socket.
 * 
 * @param msg The message string to be sent.
 * @param username The username of the recipient.
 */
void Bot::sendMsg(std::string msg, const std::string &username) {
    std::vector<std::string> messages;

    size_t pos = 0;
    while ((pos = msg.find("\n")) != std::string::npos) {
        messages.push_back(msg.substr(0, pos));
        msg.erase(0, pos + 1);
    }
    messages.push_back(msg);

    for (size_t i = 0; i < messages.size() - 1; ++i) {
        std::string reply = "PRIVMSG " + std::string(username) + " :" + messages[i] + "\r\n";
        send(sockfd, reply.c_str(), reply.length(), 0);
    }
}

/**
 * @brief Displays the received command in a formatted manner.
 * 
 * This function formats and displays the received command, the username of the sender,
 * and the arguments. If the username, command, or arguments are longer than a specified
 * maximum width, they are truncated and an ellipsis ("...") is added.
 * 
 * @param username The username of the user who issued the command.
 * @param cmd The command issued by the user.
 * @param args The arguments passed along with the command.
 */
void Bot::displayCmdReceived(std::string username, std::string cmd, std::string args) {
    const int max_width = 25;
    std::string username_display = username;
    std::string cmd_display = cmd;
    std::string args_display = args;
    
    if (args.length() > max_width)
        args_display = args.substr(0, max_width - 3) + "...";
    if (username.length() > max_width)
        username_display = username.substr(0, max_width - 3) + "...";
    if (cmd.length() > max_width)
        cmd_display = cmd.substr(0, max_width - 3) + "...";

    std::cout << "╒";
    for (int i = 0; i < (max_width - 10); ++i)
        std::cout << "═";
    std::cout << " Command received ";
    for (int i = 0; i < (max_width - 10); ++i)
        std::cout << "═";
    std::cout << "╕" << std::endl;
    std::cout << "| [Username]  => { " << std::setw(max_width) << std::left << username_display << " }   |" << std::endl;
    std::cout << "| [Command]   => { " << std::setw(max_width) << std::left << cmd_display << " }   |" << std::endl;
    std::cout << "| [Args]      => { " << std::setw(max_width) << std::left << args_display << " }   |" << std::endl;
    std::cout << "╘";
    for (int i = 0; i < max_width * 2 - 2; ++i)
        std::cout << "═";
    std::cout << "╛" << std::endl << std::endl;
}

/**
 * @brief Applies a command issued by a user.
 * 
 * This function processes a command string issued by a user, extracts the command and its arguments,
 * and invokes the corresponding command function if it exists. If the command is not found,
 * it sends a message indicating that the command was not found.
 * 
 * @param command The full command string issued by the user.
 * @param username The username of the user who issued the command.
 */
void Bot::apply_cmd(const std::string &command, const std::string &username) {
    std::string cmd = command;

    cmd = cmd.substr(0, cmd.size() - 2);
    std::string args = getArgs(cmd);
    cmd = cmd.substr(0, cmd.find(" "));
    std::map<std::string, t_data_func>::iterator it = commands.find(cmd);

    displayCmdReceived(username, cmd, args);
    if (it != commands.end()) {
        t_data_func command_data = it->second;
        CommandFunction func = command_data.func;
        sendMsg((this->*func)(username, args), username);
    } else {
        sendMsg("Command not found \nTry !help\n", username);
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
 * f
 * Initializes a socket using IPv4 and TCP. Throws an exception if the socket creation fails.
 * 
 * @throws std::runtime_error If socket creation fails.
 */
void Bot::createSocket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        close(sockfd);
        throw std::runtime_error("Socket creation error");
    }
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

    std::stringstream ss(port);
    int portNumber;
    if (!(ss >> portNumber)) {
        close(sockfd);
        throw std::runtime_error("Invalid port number");
    }
    serv_addr.sin_port = htons(portNumber);

    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sockfd);
        throw std::runtime_error("Invalid address/ Address not supported");
    }
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
    {
        close(sockfd);
        throw std::runtime_error("Connection Failed");
    }
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
            return ;
        } else if (std::string(buffer).find("001") != std::string::npos) {
            std::cout << "Successfully connected to the server" << std::endl;
            connected = true;
            return ;
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
        int poll_ret = poll(&pfd, 1, -1);
        if (poll_ret > 0) {
            return (true);
        } else if (poll_ret < 0) {
            std::cerr << "Error: poll() failed" << std::endl;
            return (false);
        }
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
    return (read(sockfd, buffer, size));
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
        std::string msg(buffer);
        std::string command;
        if (is_cmd(msg, command))
            apply_cmd(command, getUser(msg));
        else
            std::cout << "No command received" << std::endl;
    } else if (size == 0) {
        std::cerr << "Server disconnected" << std::endl;
        disconnect();
    } else {
        std::cerr << "Error: Unable to read from server" << std::endl;
        disconnect();
    }
}

// ===================================================================================================
// =========================================== Utils PART ============================================
// ===================================================================================================


// ============ Getters ============

/**
 * @brief Extracts the username from a given message.
 * 
 * This function searches for the first occurrence of the '!' character in the message
 * and extracts the substring from the start of the message up to (but not including) the '!' character.
 * 
 * @param msg The input message from which the username is to be extracted.
 * @return std::string The extracted username. Returns an empty string if '!' is not found.
 */
std::string Bot::getUser(const std::string &msg) const {
    size_t pos = msg.find("!");
    if (pos != std::string::npos) {
        return msg.substr(1, pos - 1);
    }
    return ("");
}

/**
 * @brief Extracts the arguments from a given message.
 * 
 * This function searches for the first occurrence of the ' ' (space) character in the message
 * and extracts the substring from the character immediately following the space to the end of the message.
 * 
 * @param msg The input message from which the arguments are to be extracted.
 * @return std::string The extracted arguments. Returns an empty string if a space is not found.
 */
std::string Bot::getArgs(const std::string &msg) const {
    size_t pos = msg.find(" ");
    if (pos != std::string::npos) {
        return msg.substr(pos + 1);
    }
    return ("");
}

// ============ Setters ============
