/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:24 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/07 13:46:24 by cpeterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(Server *server) : _server(server)
{
	_registered = false;
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

void	Client::sendMsg(std::string msg)
{
	msg += "\r\n";
	msg.insert(0, ":localhost ");
	send(_clientFd, msg.c_str(), msg.size(), 0);
}

void	Client::setRegistered(bool registered)
{
	if (registered == true)
		_registered = true;
	else
		_registered = false;
}

bool	Client::getRegistered()
{
	return (_registered);
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
	std::getline(iss, realname);
	if (realname.empty() || realname[1] != ':')
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		return 1;
	}
	if (_server->clientExistString(argument) != -1 && getUser() != argument)
	{
		sendMsg(ERR_ALREADYREGISTRED(argument));
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

	regex_t regex;
	regcomp(&regex, REGEXNICKNAME, REG_EXTENDED);
	if (regexec(&regex, argument.c_str(), 0, NULL, 0))
	{
		sendMsg(ERR_ERRONEUSNICKNAME(word, argument));
		regfree(&regex);
		return 1;
	}
	regfree(&regex);
	if (!error.empty())
	{
		std::cerr << RED << "Input error: " << error << RESET << std::endl;
		return 1;
	}
	if (argument.empty())
	{
		sendMsg(ERR_NONICKNAMEGIVEN(word));
		return 1;
	}
	if (_server->clientExistString(argument) != -1 && getNick() != argument)
	{
		sendMsg(ERR_NICKNAMEINUSE(argument));
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
		if (msg.empty())
			sendMsg(ERR_NOTEXTTOSEND(cmd));
		else if (target.empty())
			sendMsg(ERR_NORECIPIENT(cmd));
		return 1;
	}
	int clientIndex = _server->clientExistString(target);
	if (target.substr(0, 1) == "#")
	{
		target = target.substr(1);
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == target)
			{
				// if (_server->getChannels()[i]->receiveMsg(msg))
				// 	sendMsg(ERR_CANNOTSENDTOCHAN(cmd,target));
				return 0;
			}
		}
		sendMsg(ERR_NOSUCHCHANNEL(cmd,target));
		return 1;
	}
	if (clientIndex == -1)
		sendMsg(ERR_NOSUCHNICK(cmd,target));
	else
		_server->getClients()[clientIndex]->receiveMsg(msg);
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

	if (!error.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(word, word));
		return 1;
	}
	if (argument.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(word, word));
		return 1;
	}
	if (_server->channelExist(argument))
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			std::string name = argument;
			argument = argument.substr(1);
			if (_server->getChannels()[i]->getName() == argument)
			{
				_server->getChannels()[i]->addClient(this);
				for (size_t j = 0; j < _server->getChannels()[i]->getUserList().size(); j++)
					sendMsg(RPL_NAMREPLY(_nickname, argument, _server->getChannels()[i]->getUserList()[j]->getNick()));
				sendMsg(RPL_NAMREPLY(_nickname, argument, _nickname));
				sendMsg(RPL_ENDOFNAMES(_nickname, argument));
				sendMsg(RPL_CHANNELMODEIS(_nickname, argument, ""));
				sendMsg(RPL_NOTOPIC(_nickname, argument));
				std::string msg = ":";
				msg += _nickname;
				msg += "!";
				msg += _username;
				msg += "@127.0.0.1 JOIN ";
				msg += name;
				sendMsg(":" + _nickname + "!" + _username + " JOIN :" + argument);
			}
		}
	}
	else
	{
		Channel *new_channel = new Channel();
		new_channel->setName(argument);
		new_channel->addClient(this);
		_server->addChannel(new_channel);
		// sendMsg(":" + _nickname + "!" + _username + " JOIN :" + argument);
		std::string msg = ":";
		msg += _nickname;
		msg += "!";
		msg += _username;
		msg += "@127.0.0.1 JOIN ";
		msg += argument;
		msg += "\r\n";
		send(_clientFd, msg.c_str(), msg.size(), 0);
	}
	return 0;
}

void	Client::receiveMsg(std::string msg)
{
	sendMsg(msg);
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