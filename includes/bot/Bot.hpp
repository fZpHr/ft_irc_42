/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:26:46 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/12 21:53:28 by bberkrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <map>
# include <exception>
# include <algorithm>
# include <poll.h>
# include <iomanip>
# include <vector>
# include <string>
# include <cstdlib>

# include "TicTacToe.hpp"

# define PURPLE "\033[35m"
# define RED    "\033[31m"
# define GREEN  "\033[32m"
# define BLUE   "\033[34m"
# define BLACK  "\033[30m"
# define ORANGE "\033[33m"
# define RESET  "\033[0m"
# define GRAY "\033[37m"

class Bot
{
	public:
		// ========== Constructors ========== //
		Bot();
		Bot(const Bot &src);
		Bot &operator=(const Bot &rhs);

		// ========== Destructor ========== //
		~Bot();

		// ========== Member functions ========== //
		void 	connect(const std::string &address, const std::string &port, const std::string &password);
		void 	run();


	private:
		// ========== Connection variables ========== //
		int 			sockfd;
        bool 			connected;
        std::string 	address;
        std::string 	port;
        std::string 	password;
		
		// ========== Typedef for command functions ========== //
		typedef std::string (Bot::*CommandFunction)(const std::string&, const std::string&);

		typedef struct s_data_func
		{
			std::string description;
			CommandFunction func;
		}				t_data_func;

		// ========== Map of commands ========== //
		std::map<std::string, t_data_func> commands;
		
		// ========== Map of players tic tac toe ========== //
		std::map<std::string, TicTacToe> games;

		// ========== Commands PART functions ========== //
		std::string		tictactoe(const std::string &username, const std::string &args);
		std::string		play(const std::string &username, const std::string &args);
		std::string		restart(const std::string &username, const std::string &args);

		// ========== Help Commands PART ========== //
		std::string		help(const std::string &username, const std::string &args);
		std::string		help_list(const std::string &username, const std::string &args);
		std::string		help_tictactoe(const std::string &username, const std::string &args);
		std::string		help_play(const std::string &username, const std::string &args);
		std::string		help_restart(const std::string &username, const std::string &args);
		
		// ========== Utils Commands PART ========== //
		void 			initCommands();
		void 			addCommand(const std::string &command, const std::string &description, CommandFunction func);
		bool 			is_cmd(const std::string &msg, std::string &command);
		void 			sendMsg(std::string msg, const std::string &username);
		void			displayCmdReceived(std::string username, std::string cmd, std::string args);
		void 			apply_cmd(const std::string &command, const std::string &username);

		// ========== Connection PART functions ========== //
		void 			createSocket();
		void 			setupAddressStruct(const std::string &address, const std::string &port, struct sockaddr_in &serv_addr);
		void 			connectToServer(struct sockaddr_in &serv_addr);
		void 			sendCredentials();
		void 			handleServerResponse();
		void 			disconnect();

		// ========== Handle message PART functions ========== //
		bool 			waitForServerResponse();
		int 			readServerResponse(char *buffer, int size);
		void 			handleServerResponse(char *buffer, int size);

		// ========== Utils PART functions ========== //
		std::string 	getUser(const std::string &msg) const;
		std::string 	getArgs(const std::string &msg) const;
};

#endif