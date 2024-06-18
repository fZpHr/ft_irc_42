/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:50:32 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 18:03:50 by cpeterea         ###   ########.fr       */
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

		//SERVER
		void start();
		void socketCreation();
		void acceptClient();
		void receiveData(int fd);
		void addChannel(Channel *channel);
		static void signalHandler(int signal);
		std::vector<std::string>	handleExecCommand(std::string &command);
		int	processCommand(std::string command, int fd);
		std::string getPassword();
		void closeFds();

		//CLIENT
		int clientExistFd(int fd);
		int clientExistNick(std::string nick);
		std::vector<Client *> getClients();
		Client *getClient(std::string nick);
		void clearClients(int fd);
		void freeClients();

		//CHANNEL
		std::vector<Channel *> getChannels();
		Channel *getChannel(std::string name);
		int channelExist(std::string name);
		int channelIdx(std::string name);
		void freeChannels();
	
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