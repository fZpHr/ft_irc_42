/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/06 17:28:08 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server(int port, std::string password) : _port(port), _password(password)
{
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

std::vector<std::string> handleLogin(std::string &buffer)
{
	std::vector<std::string> parts;
	size_t pos = 0;
	while ((pos = buffer.find("\n")) != std::string::npos)
	{
		std::string part = buffer.substr(0, pos);
		std::istringstream iss(part);
		std::string cmd;
		iss >> cmd;
		if (cmd != "CAP")
			parts.push_back(part);
		buffer.erase(0, pos + 1);
	}
	if (!buffer.empty())
	{
		parts.push_back(buffer);
	}
	return parts;
}

void	Server::checkLogin(Client *client, int fd)
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
	std::string bufferStr(buffer);
	std::vector<std::string> result = handleLogin(bufferStr);
	std::string pass;
	std::string nick;
	std::string user;
	for (std::vector<std::string>::const_iterator it = result.begin(); it != result.end(); ++it)
	{
		if (it->size() >= 4 && it->substr(0, 4) == "PASS")
		{
			pass = *it;
		}
		else if (it->size() >= 4 && it->substr(0, 4) == "NICK")
		{
			nick = *it;
		}
		else if (it->size() >= 4 && it->substr(0, 4) == "USER")
		{
			user = *it;
		}
	}
	if (pass.substr(0, 4) == "PASS")
	{
		if (checkPassword(pass))
		{
			std::cout << GREEN << "Password accepted for : " << fd - 3 << RESET <<  std::endl;
			client->setPassword(true);
			if (!nick.empty() && !user.empty())
			{
				if (processCommand(nick, fd))
				{
					std::cout << GREEN << "Nick applied" << RESET << std::endl;
				}
				else
				{
					std::cout << "Received " << ret << " bytes from client " << fd - 3 << ": " << buffer << std::endl;
				}
				if (processCommand(user, fd))
				{
					std::cout << GREEN << "User applied" << RESET << std::endl;
				}
				else
				{
					std::cout << "Received " << ret << " bytes from client " << fd - 3 << ": " << buffer << std::endl;
				}
			}
		}
		else
		{
			std::cerr << BRED << ERR_PASSWDMISMATCH(pass) << RESET << std::endl;
			return;
		}
	}
	else
	{
		std::cerr << BRED << "Need password to interacte with the server" << RESET << std::endl;
		return;
	}
}

/**
 * @brief: Receive data from the client
*/
void Server::receiveData(int fd)
{
	Client *client = getClients()[clientExistFd(fd)];
	if (!client->getPassword())
	{
		checkLogin(client, fd);
		return;
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
		if (processCommand(command, fd))
		{
			std::cout << GREEN << "Command applied" << RESET << std::endl;
		}
		else
		{
			std::cout << "Received " << ret << " bytes from client " << fd - 3 << ": " << buffer << std::endl;
		}
	}
}

int	Server::processCommand(std::string command, int fd)
{
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;
	if (cmd.empty())
	{
		return 0;
	}
	iss >> command;
	int clientIndex = clientExistFd(fd);
	if (clientIndex == -1)
	{
		std::cerr << "No client found for fd " << fd << std::endl;
		return 0;
	}
	const std::string commands[5] = { "USER", "NICK" , "MSG" , "JOIN", "PASS"};
	int (Client::*functions[4])(std::string) = {&Client::setUser, &Client::setNick, &Client::prvMsg, &Client::joinChan};

	for (int i = 0; i < 4; i++) 
	{
		if (cmd == commands[i])
		{
			if (cmd == "PASS")
			{
				_clients[clientIndex]->sendMsg(ERR_ALREADYREGISTRED(command));
				return 0;
			}
			if (_clients[clientIndex]->getNick().empty() && cmd != "NICK")
			{
				std::cerr << BRED << "Need to set Nick first for: " << fd - 3 << RESET << std::endl;
				return 0;
			}
			else if (_clients[clientIndex]->getUser().empty() && cmd != "NICK" && cmd != "USER")
			{
				std::cerr << BRED << "Need to set User first for: " << fd - 3 << RESET << std::endl;
				return 0;
			}
			if ((_clients[clientIndex]->*functions[i])(command))
				return 0;
			else
				return 1;
		}
	}
	return 0;
}


// int Server::handleExecCommand(const std::string &command, int fd)
// {
// 	if (command.find("\n") != std::string::npos)
// 	{
// 		std::istringstream stream(command);
// 		std::string firstcmd;
// 		std::string secondcmd;
// 		std::getline(stream, firstcmd, '\n');
// 		std::getline(stream, firstcmd, '\n');
// 		std::getline(stream, secondcmd, '\n');
// 		if (!processCommand(firstcmd, fd))
// 			return 0;
// 		if (!processCommand(secondcmd, fd))
// 			return 0;
// 	}
// 	else
// 	{
// 		if (!processCommand(command, fd))
// 			return 0;
// 	}
// 	return 1;
// }

void Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

int Server::channelExist(std::string name)
{
	if (name[0] != '#')
		return (1);
	if (name.length() == 1)
		return (1);
	name = name.substr(1, name.length());
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
	std::istringstream iss(password);
	std::string cmd;
	std::string pass;
	iss >> cmd;
	iss >> pass;
	if (pass!= _password)
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



