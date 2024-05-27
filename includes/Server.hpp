/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:50:32 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/27 18:45:54 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "Client.hpp"


class Client;

class Server
{
	public:
		Server();
		~Server();
		Server(Server const &src);

		Server &operator=(Server const &rhs);

		void start();
		void socketCreation();
		void acceptClient();
		void receiveData(int fd);

		static void signalHandler(int signal);

		void closeFds();
		void clearClients(int fd);

	
	private:
		int _port;
		int _serverSocketFd;
		static bool _signal;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _fds;
};

#endif