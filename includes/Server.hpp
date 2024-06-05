/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:50:32 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/05 22:17:16 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Channel;

class Client;

class Server
{
	public:
		Server(int port, std::string password);
		~Server();

		void start();
		void socketCreation();
		void acceptClient();
		void receiveData(int fd);
		void addChannel(Channel *channel);

		static void signalHandler(int signal);

		// int handleExecCommand(const std::string &command, int fd);

		void closeFds();
		void clearClients(int fd);

		int channelExist(std::string name);
		int clientExistFd(int fd);
		int clientExistString(std::string name);

		std::vector<Client *> getClients();
		std::vector<Channel *> getChannels();

		void checkLogin(Client *client, int fd);

		int	processCommand(std::string command, int fd);


		int	checkPassword(std::string password);



		//DEBUG PURPOSE
		void printState();
	
	private:
		Server();
		Server(Server const &src);
		Server &operator=(Server const &rhs);
		std::vector<Channel *> _channels;
		int _port;
		int _serverSocketFd;
		static bool _signal;
		std::vector<Client *> _clients;
		std::vector<struct pollfd> _fds;
		std::string _password;
};

#endif