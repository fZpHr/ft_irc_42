/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 18:03:36 by cpeterea         ###   ########.fr       */
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

//===========================================================================================
//===================================SERV CREATION PART======================================
//===========================================================================================

/**
 * @brief: Start the server
*/
void Server::start()
{
	socketCreation(); // create the socket

	std::cout << BLACK << getCurrentTime() << "    " << YELLOW << "Server started on port " << BYELLOW << _port << RESET << std::endl;
	std::cout << BLACK << getCurrentTime() << "    " << GREEN << "Waiting for clients..." <<  RESET << std::endl;
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
	freeChannels(); // free the channels
	freeClients(); // free the clients
}

/**
 * @brief: Create a socket and bind it to the server
*/
void Server::socketCreation()
{
	struct sockaddr_in serverAddr; // create a sockaddr_in struct
	/*struct sockaddr_in {
    	short            sin_family;   // e.g. AF_INET, AF_INET6
    	unsigned short   sin_port;     // e.g. htons(6667)
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
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // set the address local address

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


//===========================================================================================
//==============================SERV CLIENT HANDLE PART======================================
//===========================================================================================

/**
 * @brief: Accept a client connection
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
		std::cerr << BLACK << getCurrentTime() << "    " << "Accept failed" << std::endl;
		delete newClient;
		return;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) // set the client socket to non-blocking
	{
		std::cerr << BLACK << getCurrentTime() << "    " << "Fcntl failed" << std::endl;
		delete newClient;
		return;
	}

	pollFd.fd = clientFd; // set the file descriptor of the pollfd to the client file descriptor
	pollFd.events = POLLIN; // set the events to POLLIN
	pollFd.revents = 0; // set the returned events to 0

	newClient->set_fd(clientFd); // set the client file descriptor
	newClient->set_IPclient(inet_ntoa(clientAddr.sin_addr)); // set the client IP

	_clients.push_back(newClient); // add the client to the client vector
	_fds.push_back(pollFd); // add the pollfd to the vector

	std::cout << BLACK << getCurrentTime() << "    " << GREEN << "New client " << clientFd - 3 << " from " << newClient->get_IPclient() << RESET <<std::endl;
}

/**
 * @brief: Receive data from the client
 * @param fd: the file descriptor of the client
*/
void Server::receiveData(int fd)
{
	int clientIndex = clientExistFd(fd);
	Client *client = getClients()[clientIndex];
	
	char buffer[4096]; // create a buffer to store the data
	memset(buffer, 0, 4096); // set the buffer to 0
	
	ssize_t ret = recv(fd, buffer, 4096, 0); // receive the data from the client
	if (ret  <= 0) // if the client disconnected
	{
		if (_clients[clientIndex]->getAlreadyInChannel())
		{
			for (size_t i = 0; i < _channels.size(); i++)
			{
				std::vector<Client *> lst = getChannels()[i]->getUserList();
				for (size_t j = 0; j != lst.size(); j++)
				{
					std::string msg = ":";
					msg += _clients[clientIndex]->getNick();
					msg += "!";
					msg += _clients[clientIndex]->getUser();
					msg += "@127.0.0.1 PART ";
					msg += getChannels()[i]->getName();
					msg += " :Leaving";
					msg += "\r\n";
					send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
				}
				_channels[i]->removeClient(_clients[clientIndex]);
				_channels[i]->removeUserMod(_clients[clientIndex]);
			}
			_clients[clientIndex]->setAlreadyInChannel(false);
		}
		std::cout << BLACK << getCurrentTime() << "    " << RED << "Client " << fd - 3 << " disconnected" << RESET << std::endl;
		clearClients(fd); // remove the client from the pollfd vector and the client vector
		close(fd); // close the client socket
		delete client;
		return;
	}
	buffer[ret] = '\0'; // add a null terminator to the buffer
	std::string command(buffer);
	std::vector<std::string> commands = handleExecCommand(command);

	for (std::vector<std::string>::const_iterator it = commands.begin(); it != commands.end(); ++it)
	{
		std::string command = *it;
		int res = processCommand(command, fd);
		if (res == 1)
		{
			std::cout << BLACK << getCurrentTime() << "    " << CYAN << "Command applied" << RESET << std::endl;
			if (!client->getUser().empty() && client->getRegistered() == false && !client->getNick().empty() && client->getPassword() == true)
			{
				_clients[clientExistFd(fd)]->sendMsg(RPL_WELCOME(_clients[clientExistFd(fd)]->getNick()));
				_clients[clientExistFd(fd)]->sendMsg(RPL_YOURHOST(_clients[clientExistFd(fd)]->getNick(), "localhost",  "1.0"));
				_clients[clientExistFd(fd)]->sendMsg(RPL_CREATED(_clients[clientExistFd(fd)]->getNick(), "-3000 av JC"));
				_clients[clientExistFd(fd)]->setRegistered(true);
				std::cout << BLACK << getCurrentTime() << "    " << GREEN << "Client " << fd - 3 << " registered" << RESET << std::endl;
			}
		}
		else if (res == 2)
			break;
		else
		{
			std::cout << BLACK << getCurrentTime() << "    " << DARK << "Received " << ret << " bytes from client " << fd - 3 << ": " << command << RESET << std::endl;
		}
	}
}

/**
 * @brief: Process the command received from the client
 * @param command: the command received
 * @param fd: the file descriptor of the client
*/
int	Server::processCommand(std::string command, int fd)
{
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;
	int clientIndex = clientExistFd(fd);
	const std::string commands[10] = { "USER", "NICK" , "PRIVMSG" , "JOIN", "PASS", "PART", "KICK", "INVITE", "MODE", "TOPIC"};
	int (Client::*functions[10])(std::string) = {&Client::setUser, &Client::setNick, &Client::prvMsg, &Client::joinChan, &Client::setPassword, &Client::leaveChan, &Client::kickChan, &Client::inviteChan, &Client::modChan, &Client::topic};

	for (int i = 0; i < 10; i++) 
	{
		if (cmd == commands[i])
		{
			int res = (_clients[clientIndex]->*functions[i])(command);
			if (res == 1)
				return 0;
			else
				return 1;
		}
		else if (cmd == "QUIT")
		{
			if (_clients[clientIndex]->getAlreadyInChannel())
			{
				for (size_t i = 0; i < _channels.size(); i++)
				{
					std::vector<Client *> lst = getChannels()[i]->getUserList();
					for (size_t j = 0; j != lst.size(); j++)
					{
						std::string msg = ":";
						msg += _clients[clientIndex]->getNick();
						msg += "!";
						msg += _clients[clientIndex]->getUser();
						msg += "@127.0.0.1 PART ";
						msg += getChannels()[i]->getName();
						msg += " :Leaving";
						msg += "\r\n";
						send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
					}
					_channels[i]->removeClient(_clients[clientIndex]);
					_channels[i]->removeUserMod(_clients[clientIndex]);
				}
				_clients[clientIndex]->setAlreadyInChannel(false);
			}
			std::cout << BLACK << getCurrentTime() << "    " << RED << "Client " << fd - 3 << " disconnected" << RESET << std::endl;
			return 2;
		}
	}
	return 0;
}


//===========================================================================================
//======================================UTILS SERV PART======================================
//===========================================================================================

/**
 * @brief: close all clients (fds) and the server socket
*/
void	Server::closeFds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << BLACK << getCurrentTime() << "    " << RED << "Closing client " << _clients[i]->get_fd() - 3 << RESET << std::endl;
		close(_fds[i].fd);
		close(_clients[i]->get_fd());
	}
	if (_serverSocketFd != -1)
		close(_serverSocketFd);
}

/**
 * @brief: Signal handler for SIGINT
*/
void Server::signalHandler(int signal)
{
	if (signal == SIGINT)
		std::cout << BLACK << getCurrentTime() << "    " << RED << "ctrl + C received" << RESET << std::endl;
	else if (signal == SIGQUIT)
		std::cout << BLACK << getCurrentTime() << "    " << RED << "ctrl + \\ received" << RESET << std::endl;
	_signal = true;
}

/**
 * @brief: Handle the command received from the client
 * @param command the command to handle
 * @return the parts of the command
*/
std::vector<std::string>	Server::handleExecCommand(std::string &command)
{
	std::vector<std::string> result;
	size_t pos;
	while ((pos = command.find("\r\n")) != std::string::npos)
	{
		std::string part = command.substr(0, pos);
		result.push_back(part);
		command.erase(0, pos + 2);
	}
	if (!command.empty()) {
		result.push_back(command);
	}
	return result;
}

std::string Server::getPassword()
{
	return (_password);
}

//=============CLIENT PART==================

/**
 * @brief: Get the client from the client vector with the nick
 * @param nick: the nick of the client
 * @return: the client if found, NULL otherwise
*/
Client *Server::getClient(std::string nick)
{
	for (std::vector<Client *>::iterator itr = _clients.begin(); itr != _clients.end(); ++itr)
	{
		if ((*itr)->getNick() == nick)
			return (*itr);
	}
	return (NULL);
}

/**
 * @brief: Remove the client from the pollfd vector and the client vector
*/
void Server::clearClients(int fd)
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
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
 * @brief: Check if the nick already exists
 * @param nick the nick to check
 * @return the index of the client if found, -1 otherwise
*/
int Server::clientExistNick(std::string nick)
{
	if (nick.empty())
		return (-1);
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNick() == nick)
		{
			return (i);
		}
	}
	return (-1);
}

/**
 * @brief: Check if the fd already exists
 * @param fd the fd to check
 * @return the index of the client if found, -1 otherwise
*/
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

std::vector<Client *> Server::getClients()
{
	return (_clients);
}

void Server::freeClients()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		delete _clients[i];
	}
}

//===============CHANNEL PART================

void Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

/**
 * @brief: Get the index of the channel
 * @param name the name of the channel
 * @return the index of the channel if found, -1 otherwise
*/
int	Server::channelIdx(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return (i);
	}
	return (-1);
}

/**
 * @brief: Check if the channel exists
 * @param name the name of the channel
 * @return 1 if the channel exists, 0 otherwise
*/
int Server::channelExist(std::string name)
{
	if (name.length() == 1)
		return (0);
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return (1);
	}
	return (0);
}

/**
 * @brief: Get the channel from the server
 * @param name the name of the channel
 * @return the channel if found, NULL otherwise
*/
Channel *Server::getChannel(std::string name)
{
	for (size_t i = 0; i != _channels.size(); i++)
	{
		if (name == _channels[i]->getName())
		{
			return (_channels[i]);
		}
	}
	return NULL;
}

std::vector<Channel *> Server::getChannels()
{
	return (_channels);
}

void Server::freeChannels()
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		delete _channels[i];
	}
}
