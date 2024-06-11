/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:24 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/11 15:42:38 by hbelle           ###   ########.fr       */
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

//##############################################################################################################
//##############################################################################################################
//##############################################################################################################
//############################################COMMANDS CLIENT PART##############################################
//##############################################################################################################
//##############################################################################################################
//##############################################################################################################

int	Client::setUser(std::string name)
{
	// if (_password == false)
	// {
	// 	sendMsg(ERR_NOTREGISTERED(_nickname));
	// 	std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must set a password first" << RESET << std::endl;
	// 	return 1;
	// }
	// if (_nickname.empty())
	// {
	// 	sendMsg(ERR_NOTREGISTERED(_nickname));
	// 	std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must set a nickname first" << RESET << std::endl;
	// 	return 1;
	// }
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
	realname = realname.substr(1);
	if (_registered)
	{
		sendMsg(ERR_ALREADYREGISTRED(argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Can't change credential after being registered" << RESET << std::endl;
		return 1;
	}
	if (realname.empty() || realname[0] != ':')
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
	_username = argument;
	return 0;
}

int  Client::setNick(std::string nick)
{
	// if (_password == false)
	// {
	// 	sendMsg(ERR_NOTREGISTERED(_nickname));
	// 	std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must set a password first" << RESET << std::endl;
	// 	return 1;
	// }
	std::istringstream iss(nick);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	getline(iss, argument);
	argument = argument.substr(1);
	regex_t regex;
	int ret;
	ret = regcomp(&regex, REGEXNICKNAME, REG_EXTENDED);
	if (ret) {
		char msgbuf[100];
		regerror(ret, &regex, msgbuf, sizeof(msgbuf));
		std::cerr << BLACK << getCurrentTime() << "    " << "Regex error: " << msgbuf << std::endl;
		return 1;
	}
	if (regexec(&regex, argument.c_str(), 0, NULL, 0))
	{
		sendMsg(ERR_ERRONEUSNICKNAME(word, argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Invalid nickname" << RESET << std::endl;
		regfree(&regex);
		return 1;
	}
	regfree(&regex);
	if (!error.empty())
	{
		sendMsg(ERR_ERRONEUSNICKNAME(word, argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Too many arguments" << RESET << std::endl;
		return 1;
	}
	if (argument.empty())
	{
		sendMsg(ERR_NONICKNAMEGIVEN(word));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No nickname given" << RESET << std::endl;
		return 1;
	}
	if (_server->clientExistString(argument) != -1 && getNick() != argument)
	{
		sendMsg(ERR_NICKNAMEINUSE(argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Nickname already in use" << RESET << std::endl;
		return 1;
	}
	_nickname = argument;
	return 0;
}


int	Client::prvMsg(std::string input)
{
	if (_username.empty())
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	std::istringstream iss(input);
	std::string cmd;
	std::string target;
	std::string msg;
	iss >> cmd;
	iss >> target;
	std::getline(iss, msg);
	msg = msg.substr(1);
	if ( msg.empty() || target.empty())
	{
		if (msg.empty())
		{
			sendMsg(ERR_NOTEXTTOSEND(cmd));
			std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No text to send" << RESET << std::endl;
		}
		else
		{
			sendMsg(ERR_NORECIPIENT(cmd));
			std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No recipient given" << RESET << std::endl;
		}
		return 1;
	}
	int clientIndex = _server->clientExistString(target);
	if (target.substr(0, 1) == "#")
	{
		std::cout << "target: " << target << std::endl;
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			std::cout << "channel: " << _server->getChannels()[i]->getName() << std::endl;
			if (_server->getChannels()[i]->getName() == target)
			{
				// if (_server->getChannels()[i]->receiveMsg(msg))
				// 	sendMsg(ERR_CANNOTSENDTOCHAN(cmd,target));
				return 0;
			}
		}
		sendMsg(ERR_NOSUCHCHANNEL(cmd,target));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such channel" << RESET << std::endl;
		return 1;
	}
	if (clientIndex == -1)
	{
		sendMsg(ERR_NOSUCHNICK(cmd,target));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such nick/channel" << RESET << std::endl;
		return 1;
	}
	else
	{
		std::string realsend;
		if (msg[0] == ':')
			realsend += ":" + _nickname + "!" + _username + "@localhost PRIVMSG " + target + " " + msg + "\r\n";
		else
			realsend += ":" + _nickname + "!" + _username + "@localhost PRIVMSG " + target + " :" + msg + "\r\n";
		send(_server->getClients()[clientIndex]->get_fd(), realsend.c_str(), realsend.size(), 0);
	}
	return 0;
}

int	Client::joinChan(std::string target)
{
	if (_username.empty())
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	std::istringstream iss(target);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	iss >> argument;
	iss >> error;

	if (!error.empty())
	{
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Too many arguments" << RESET << std::endl;
		return 1;
	}
	if (argument.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(word, word));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
	if (_server->channelExist(argument))
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			std::string name = argument;
			if (_server->getChannels()[i]->getName() == name)
			{
				_server->getChannels()[i]->addClient(this);
				std::string msg = ":";
				msg += _nickname;
				msg += "!";
				msg += _username;
				msg += "@127.0.0.1 JOIN ";
				msg += argument;
				msg += "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
				std::vector<Client *> lst = _server->getChannels()[i]->getUserList();
				for (size_t j = 0; j != lst.size(); j++)
				{
					std::string msg = ":";
					msg += _nickname;
					msg += "!";
					msg += _username;
					msg += "@127.0.0.1 JOIN ";
					msg += argument;
					msg += "\r\n";
					send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
				}
				sendMsg(RPL_NAMREPLY(_nickname, argument, _server->getChannels()[i]->getNicks()));
				sendMsg(RPL_ENDOFNAMES(_nickname, argument));
				sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
				sendMsg(RPL_NOTOPIC(_nickname, argument));
			}
		}
	}
	else
	{
		Channel *new_channel = new Channel();
		new_channel->setName(argument);
		new_channel->addClient(this);
		new_channel->addUserMod(this);
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
		msg = ":";
		msg += _nickname;
		msg += "!";
		msg += _username;
		msg += "@127.0.0.1 MODE ";
		msg += argument;
		msg += " +o ";
		msg += _nickname;
		msg += "\r\n";
		send(_clientFd, msg.c_str(), msg.size(), 0);
		sendMsg(RPL_NAMREPLY(_nickname, argument, "@"+_nickname));
		sendMsg(RPL_ENDOFNAMES(_nickname, argument));
		sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
		sendMsg(RPL_NOTOPIC(_nickname, argument));
	}
	return 0;
}

int Client::leaveChan(std::string target)
{
	
	(void)target;
	return 0;
}

int Client::kickChan(std::string target, std::string channel, std::string reason)
{
	(void)target;
	(void)channel;
	(void)reason;
	return 0;
}

int	Client::setPassword(std::string command)
{
	std::istringstream iss(command);
	std::string cmd;
	std::string pass;
	iss >> cmd;
	getline(iss, pass);
	pass = pass.substr(1);
	if (_registered == true)
	{
		sendMsg(ERR_ALREADYREGISTRED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Can't change credential after being registered" << RESET << std::endl;
		return 1;
	}
	else if (pass == _server->getPassword())
	{
		_password = true;
		return 0;
	}
	else
	{
		sendMsg(ERR_PASSWDMISMATCH(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Password incorrect" << RESET << std::endl;
		return 1;
	}
}

//##############################################################################################################
//##############################################################################################################
//##############################################################################################################
//############################################UTILS CLIENT PART#################################################
//##############################################################################################################
//##############################################################################################################
//##############################################################################################################


int	Client::setPerms()
{
	if (_perms == true)
	{
		sendMsg(ERR_ALREADYREGISTRED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You have already set permissions" << RESET << std::endl;
		return 1;
	}
	_perms = true;
	return 0;
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