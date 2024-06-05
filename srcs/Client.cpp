/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:24 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/05 22:05:16 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(Server *server) : _server(server)
{
	_perms = false;
	_password = false;
	_username = "";
	_nickname = "";
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
	std::string cmd;
	std::string argument;
	std::string hostname;
	std::string servername;
	std::string realname;
	std::string error;
	iss >> cmd;
	iss >> argument;
	iss >> hostname;
	iss >> servername;
	iss >> realname;
	iss >> error;

	if (!error.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	else if (realname.empty())
	{
		std::cerr << RED << "Input error" << RESET << std::endl;
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
	if (_server->clientExistString(argument) != -1)
	{
		std::cerr << RED << ERR_NICKNAMEINUSE(argument, argument) << RESET << std::endl;
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
	std::string cmd;
	std::string target;
	std::string msg;
	std::string error;
	iss >> cmd;
	iss >> target;
	iss >> msg;
	iss >> error;

	if (!error.empty() || msg.empty() || target.empty())
	{
		std::cerr << RED << "Input error" << RESET << std::endl;
		return 1;
	}
	int clientIndex = _server->clientExistString(target);
	if (clientIndex != -1)
	{
		_server->getClients()[clientIndex]->receiveMsg(msg);
	}
	else
	{
		std::cerr << RED << "Client does not exist: " << target << RESET << std::endl;
		return 1;
	}
	
	return 0;
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
	if (!_server->channelExist(argument))
    {
        std::cerr << RED << "Channel does not exist: " << argument << RESET << std::endl;
        return 1;
    }
	for (size_t i = 0; i < _server->getChannels().size(); i++)
	{
		if (_server->getChannels()[i]->getName() == argument)
		{
			_server->getChannels()[i]->addClient(this);
			std::cout << "Client " << _nickname << " joined channel " << argument << std::endl;
		}
	}
	return 0;
}

void	Client::receiveMsg(std::string msg)
{
	std::cout << msg << std::endl;
}

std::string	Client::getUser()
{
	return (_username);
}

std::string	Client::getNick()
{
	return (_nickname);
}

bool	Client::getPerms()
{
	return (_perms);
}

bool	Client::getPassword()
{
	return (_password);
}

void	Client::setPassword(bool password)
{
	if (password == true)
		_password = true;
	else
		_password = false;
}