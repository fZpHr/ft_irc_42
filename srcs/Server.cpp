/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/05 16:51:23 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server()
{
	_port = 6667;
	_password = "slt";
	_serverSocketFd = -1;
}

Server::~Server()
{
}

Server::Server(Server const &src)
{
	*this = src;
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
	}
	return (*this);
}


/**
 * @brief: Remove the client from the pollfd vector and the client vector
*/
void Server::clearClients(int fd)
{
	//Remove the client from the pollfd vector
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}

	//Remove the client from the client vector
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->get_fd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

/**
 * @brief: close all clients (fds) and the server socket (fd != -1)
*/
void	Server::closeFds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << RED << "Closing client " << _clients[i]->get_fd() - 3 << RESET << std::endl;
		close(_fds[i].fd);
	}
	if (_serverSocketFd != -1)
		close(_serverSocketFd);
}

/**
 * @brief: Signal handler for SIGINT
*/
void Server::signalHandler(int signal)
{
	(void)signal;
	if (signal == SIGINT)
		std::cout << std::endl << RED << "ctrl + C received" << RESET << std::endl;
	else if (signal == SIGQUIT)
		std::cout << std::endl << RED << "ctrl + \\ received" << RESET << std::endl;
	_signal = true;
}

/**
 * @brief: Create a socket and bind it to the server
*/
void Server::socketCreation()
{
	struct sockaddr_in serverAddr; // create a sockaddr_in struct
	/*struct sockaddr_in {
    	short            sin_family;   // e.g. AF_INET, AF_INET6
    	unsigned short   sin_port;     // e.g. htons(3490)
    	struct in_addr   sin_addr;     // see struct in_addr, below
		sruct in_addr {
    		unsigned long s_addr;  // load with inet_aton()
		};
    	char             sin_zero[8];  // zero this if you want to
	};*/
	struct pollfd pollFd; // create a pollfd struct
	/*struct pollfd {
    	int   fd;         file descriptor 
    	short events;     requested events 
    	short revents;    returned events 
	};*/

	_serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // create a socket (int domain, int type, int protocol), AF_INET is the address family
																	// for IPV4, Common values include SOCK_STREAM for TCP sockets (providing reliable, bidirectional, byte-stream communication) 
																	//, IPPROTO_TCP is the protocol
	if (_serverSocketFd == -1) // check if the socket was created
		throw std::runtime_error("Socket creation failed");

	serverAddr.sin_family = AF_INET; // set family to IPV4
	serverAddr.sin_port = htons(_port); // convert the port to network byte order
	serverAddr.sin_addr.s_addr = INADDR_ANY; // set the address to any local address

	int tmp = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) == -1) // set the socket to reuse the address immediatly 
																							// after a reboot and avoid the "Address already in use" error
		throw std::runtime_error("Setsockopt failed");
		
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1) // manipulate a socket to add non-blocking (exit immediately and throw an error
														// if the socket is not ready to perform the requested operation)
		throw std::runtime_error("Fcntl failed");
		
	if (bind(_serverSocketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) // link the socket to the server
		throw std::runtime_error("Bind failed");

	if (listen(_serverSocketFd, SOMAXCONN) == -1) // put the socket in listening mode for incoming connections, SOMAXCONN is the maximum
													// number of pending connections that can be queued
		throw std::runtime_error("Listen failed");

	pollFd.fd = _serverSocketFd; // add the server socket to the pollfd vector
	pollFd.events = POLLIN; // set the events we want to POLLIN (alert when data is available to read)
	pollFd.revents = 0;  // set the returned events to 0 (no events returned yet, will be updated by poll)
	_fds.push_back(pollFd); // add the pollfd to the vector 
}

/**
 * @brief: Receive data from the client
*/
void Server::acceptClient()
{
	
	Client *newClient = new Client(this); // create a new client
	struct sockaddr_in clientAddr; // create a sockaddr_in struct
	struct pollfd pollFd; // create a pollfd struct
	socklen_t clientAddrSize = sizeof(clientAddr); // set the size of the client address

	int clientFd = accept(_serverSocketFd, (struct sockaddr *)&clientAddr, &clientAddrSize); // Block until a client connects to the server, 
																							// return the client file descriptor
	if (clientFd == -1) // check if the client was accepted
	{
		std::cerr << "Accept failed" << std::endl;
		return;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) // set the client socket to non-blocking
	{
		std::cerr << "Fcntl failed" << std::endl;
		return;
	}

	pollFd.fd = clientFd; // set the file descriptor of the pollfd to the client file descriptor
	pollFd.events = POLLIN; // set the events to POLLIN
	pollFd.revents = 0; // set the returned events to 0

	newClient->set_fd(clientFd); // set the client file descriptor
	newClient->set_IPclient(inet_ntoa(clientAddr.sin_addr)); // set the client IP

	_clients.push_back(newClient); // add the client to the client vector
	_fds.push_back(pollFd); // add the pollfd to the vector

	std::cout << GREEN << "New client " << clientFd - 3 << " from " << newClient->get_IPclient() << RESET <<std::endl;
}



int Server::clientExistFd(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->get_fd() == fd)
		{
			return (i);
		}
	}
	return (-1);
}

int Server::clientExistString(std::string name)
{
	if (name.empty())
		return (-1);
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getUser() == name || _clients[i]->getNick() == name)
		{
			std::cout << GREEN << "Client found: " << _clients[i]->getUser() << RESET << std::endl;
			return (i);
		}
	}
	return (-1);
}


/**
 * @brief: Receive data from the client
*/
void Server::receiveData(int fd)
{
	Client *client = getClients()[clientExistFd(fd)];
	if (!client->getPassword())
	{
		char buffer[4096]; // create a buffer to store the data
		memset(buffer, 0, 4096); // set the buffer to 0
		ssize_t ret = recv(fd, buffer, 4096, 0); // receive the data from the client
		if (ret  <= 0) // if the client disconnected
		{
			std::cout << RED << "Client " << fd - 3 << " disconnected" << RESET << std::endl;
			clearClients(fd); // remove the client from the pollfd vector and the client vector
			close(fd); // close the client socket
			return;
		}
		buffer[ret] = '\0'; // add a null terminator to the buffer
		std::istringstream iss(buffer);
		std::string cmd;
		std::string password;
		iss >> cmd;
		iss >> password;
		if (cmd == "PASS")
		{

			if (checkPassword(password))
			{
				std::cout << BGREEN << "Password accepted for :" << fd - 3 << RESET <<  std::endl;
				client->setPassword(true);
			}
			else
			{
				std::cerr << BRED << ERR_PASSWDMISMATCH(password) << RESET << std::endl;
				return;
			}
		}
		else
		{
			std::cerr << BRED << "Need password to interacte with the server" << RESET << std::endl;
			return;
		}
	}
	else
	{
		char buffer[4096]; // create a buffer to store the data
		memset(buffer, 0, 4096); // set the buffer to 0
		
		ssize_t ret = recv(fd, buffer, 4096, 0); // receive the data from the client
		if (ret  <= 0) // if the client disconnected
		{
			std::cout << RED << "Client " << fd - 3 << " disconnected" << RESET << std::endl;
			clearClients(fd); // remove the client from the pollfd vector and the client vector
			close(fd); // close the client socket
			return;
		}
		buffer[ret] = '\0'; // add a null terminator to the buffer
		std::string command(buffer);

		if (handleExecCommand(command, fd))
		{
			std::cout << BGREEN << "Command applied" << RESET << std::endl;
		}
		else
		{
			std::cout << "Received " << ret << " bytes from client " << fd - 3 << ": " << buffer << std::endl;
		}
	}
}

int Server::handleExecCommand(const std::string &command, int fd)
{

	int clientIndex = clientExistFd(fd);
	if (clientIndex == -1)
	{
		std::cerr << "No client found for fd " << fd << std::endl;
		return 0;
	}
	std::istringstream iss(command);
	std::string word;
	iss >> word;

	const std::string commands[4] = { "USER", "NICK" , "MSG" , "JOIN"};
	int (Client::*functions[4])(std::string) = {&Client::setUser, &Client::setNick, &Client::prvMsg, &Client::joinChan};

	for (int i = 0; i < 4; i++) 
	{
		if (word == commands[i])
		{
			if ((_clients[clientIndex]->*functions[i])(command))
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

void Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

int Server::channelExist(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return (1);
	}
	return (0);
}

std::vector<Client *> Server::getClients()
{
	return (_clients);
}

std::vector<Channel *> Server::getChannels()
{
	return (_channels);
}

int Server::checkPassword(std::string password)
{
	if (password != _password)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

/**
 * @brief: Start the server
*/
void Server::start()
{
	socketCreation(); // create the socket

	std::cout << YELLOW << "Server started on port " << BYELLOW << _port << RESET << std::endl;
	std::cout << GREEN << "Waiting for clients..." <<  RESET << std::endl;
	while (!_signal) // while the signal is not received
	{
		if ((poll(_fds.data(), _fds.size(), -1) == -1) && Server::_signal == false ) // wait for events on the pollfd vector, -1 means infinite timeout
			throw std::runtime_error("Poll failed");

		for (size_t i = 0; i < _fds.size(); i++) // loop through the pollfd vector
		{
			if (_fds[i].revents & POLLIN) // if the returned events are POLLIN
			{
				if (_fds[i].fd == _serverSocketFd) // if the file descriptor is the server socket
					acceptClient(); // accept the client
				else
					receiveData(_fds[i].fd); // receive data from the client
			}
		}
	}
	closeFds(); // close all clients and the server socket
}




//DEBUG PURPOSE

void Server::printState()
{
	std::cout << "Clients: " << _clients.size() << std::endl;
	std::cout << GREEN << "-----------" << RESET << std::endl;
	if (_clients.size() > 0)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			std::cout << "Client nick: " << _clients[i]->getNick() << std::endl;
			std::cout << "Client user: " << _clients[i]->getUser() << std::endl;
			std::cout << "Client fd: " << _clients[i]->get_fd() << std::endl;
			std::cout << "Client IP: " << _clients[i]->get_IPclient() << std::endl;
			std::cout << "Client perms: " << _clients[i]->getPerms() << std::endl;
			if (i < _clients.size() - 1)
				std::cout << GREEN << "-----------" << RESET << std::endl;
		}
	}
	std::cout << YELLOW << "------------------------" << RESET << std::endl;
	std::cout << "Channels: " << _channels.size() << std::endl;
	std::cout << GREEN << "-----------" << RESET << std::endl;
	if (_channels.size() > 0)
	{
		for (size_t i = 0; i < _channels.size(); i++)
		{
			std::cout << "Channel name: " << _channels[i]->getName() << std::endl;
			std::cout << "Channel topic: " << _channels[i]->getTopic() << std::endl;
			// std::cout << "Channel user limit: " << _channels[i]->getUserLimit() << std::endl;
			// std::cout << "Channel private: " << _channels[i]->getPrivate() << std::endl;
			if (i < _channels.size() - 1)
				std::cout << GREEN << "-----------" << RESET << std::endl;
		}
	}
}



