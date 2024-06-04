/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:50:32 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/04 18:22:57 by hbelle           ###   ########.fr       */
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
		Server();
		~Server();

		void start();
		void socketCreation();
		void acceptClient();
		void receiveData(int fd);
		void addChannel(Channel *channel);

		static void signalHandler(int signal);

		int handleExecCommand(const std::string &command, int fd);

		void closeFds();
		void clearClients(int fd);

		int channelExist(std::string name);
		int clientExistFd(int fd);
		int clientExistString(std::string name);

		std::vector<Client *> getClients();
		std::vector<Channel *> getChannels();


		//DEBUG PURPOSE
		void printState();
	
	private:
		Server(Server const &src);
		Server &operator=(Server const &rhs);
		std::vector<Channel *> _channels;
		int _port;
		int _serverSocketFd;
		static bool _signal;
		std::vector<Client *> _clients;
		std::vector<struct pollfd> _fds;
};

#endif