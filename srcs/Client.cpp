/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:24 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/03 19:26:27 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
	_perms = false;
}

Client::~Client()
{
}

Client::Client(Client const &src)
{
	*this = src;
}

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
	}
	return (*this);
}

int		Client::get_fd() const
{
	return (_clientFd);
}

void	Client::set_fd(int fd)
{
	_clientFd = fd;
}

std::string Client::get_IPclient() const
{
	return (_IPclient);
}

void Client::set_IPclient(std::string IPclient)
{
	this->_IPclient = IPclient;
}

int	Client::setUser(std::string name)
{
	std::istringstream iss(name);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	iss >> argument;
	iss >> error;

	if (!error.empty() || argument.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	_username = argument;
	return 0;
}

int  Client::setNick(std::string nick)
{
	std::istringstream iss(nick);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	iss >> argument;
	iss >> error;

	if (!error.empty() || argument.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	_nickname = argument;
	return 0;
}

void	Client::setPerms(bool perms)
{
	if (perms == true)
		_perms = true;
	else
		_perms = false;
}


int	Client::prvMsg(std::string input)
{
	std::istringstream iss(input);
	std::string user;
	std::string target;
	std::string msg;
	std::string error;
	iss >> user;
	iss >> target;
	iss >> msg;
	iss >> error;

	if (!error.empty() || msg.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	(void) target;
	(void) msg;
	std::cerr << RED << "DEV IN PROGRESS" << RESET << std::endl;
	return 0;
	//sendMsg(target, msg);
}

int	Client::joinChan(std::string target)
{
	std::istringstream iss(target);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	iss >> argument;
	iss >> error;

	if (!error.empty() || argument.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	std::cerr << RED << "DEV IN PROGRESS" << RESET << std::endl;
	(void)target;
	return 0;
}
