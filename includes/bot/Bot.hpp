/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:26:46 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/11 14:25:24 by ben              ###   ########.fr       */
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
		Bot();
		Bot(const Bot &src);
		~Bot();

		Bot &operator=(const Bot &rhs);

		void connect(const std::string &address, const std::string &port, const std::string &password);
		void run();


	private:
		int sockfd;
        bool connected;
        std::string address;
        std::string port;
        std::string password;

		// Connection functions
		void createSocket();
		void setupAddressStruct(const std::string &address, const std::string &port, struct sockaddr_in &serv_addr);
		void connectToServer(struct sockaddr_in &serv_addr);
		void sendCredentials();
		void handleServerResponse();
		void disconnect();

		// Handle message functions
		bool waitForServerResponse();
		int readServerResponse(char *buffer, int size);
		void handleServerResponse(char *buffer, int size);

		typedef std::string (Bot::*CommandFunction)();
		std::map<std::string, CommandFunction> commands;

		void initCommands();

		bool is_cmd(const std::string &msg, std::string &command);
		void apply_cmd(const std::string &command);

		std::string help();
};

#endif