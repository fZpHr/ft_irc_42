/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/28 21:27:22 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server()
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
		if (_clients[i].get_fd() == fd)
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
		std::cout << RED << "Closing client " << _clients[i].get_fd() << RESET << std::endl;
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
	Client newClient; // create a new client
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

	newClient.set_fd(clientFd); // set the client file descriptor
	newClient.set_IPclient(inet_ntoa(clientAddr.sin_addr)); // set the client IP

	_clients.push_back(newClient); // add the client to the client vector
	_fds.push_back(pollFd); // add the pollfd to the vector

	std::cout << GREEN << "New client " << clientFd - 3 << " from " << newClient.get_IPclient() << RESET <<std::endl;
}

/**
 * @brief: Receive data from the client
*/
void Server::receiveData(int fd)
{
	char buffer[4096]; // create a buffer to store the data
	memset(buffer, 0, 4096); // set the buffer to 0
	
	ssize_t ret = recv(fd, buffer, 4096, 0); // receive the data from the client
	if (ret  <= 0) // if the client disconnected
	{
		std::cout << RED << "Client " << fd << " disconnected" << RESET << std::endl;
		clearClients(fd); // remove the client from the pollfd vector and the client vector
		close(fd); // close the client socket
		return;
	}
	buffer[ret] = '\0'; // add a null terminator to the buffer
	std::cout << YELLOW << "Received " << ret << " bytes from client " << fd - 3 << ": " << buffer << RESET << std::endl;
}

/**
 * @brief: Start the server
*/
void Server::start()
{
	_port = 6667; // set the port to 6667 (standard port)
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




