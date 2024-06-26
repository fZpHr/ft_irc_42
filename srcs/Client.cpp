/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:52:24 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 18:00:19 by cpeterea         ###   ########.fr       */
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
	_alreadyInChannel = false;
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

// ===================================================================================================
// ======================================CLIENT PART==================================================
// ===================================================================================================


// ===========COMMANDS LOGIN==================

/**
 * @brief: set the username of the client
 * @param name the username to set
 * @return 0 if no error, 1 otherwise
*/
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
	if (_registered)
	{
		sendMsg(ERR_ALREADYREGISTRED(argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Can't change credential after being registered" << RESET << std::endl;
		return 1;
	}
	if (realname.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
	realname = realname.substr(1);
	if (realname.empty() || realname[0] != ':')
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
	_username = argument;
	return 0;
}

/**
 * @brief: set the nickname of the client
 * @param nick the nickname to set
 * @return 0 if no error, 1 otherwise
*/
int  Client::setNick(std::string nick)
{
	std::istringstream iss(nick);
	std::string word;
	std::string argument;
	std::string error;
	iss >> word;
	getline(iss, argument);
	regex_t regex;
	int ret;
	if (argument.empty())
	{
		sendMsg(ERR_NONICKNAMEGIVEN(word));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No nickname given" << RESET << std::endl;
		return 1;
	}
	argument = argument.substr(1);
	ret = regcomp(&regex, REGEXNICKNAME, REG_EXTENDED);
	if (ret)
	{
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
	if (_server->clientExistNick(argument) != -1 && getNick() != argument)
	{
		sendMsg(ERR_NICKNAMEINUSE(argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Nickname already in use" << RESET << std::endl;
		return 1;
	}
	_nickname = argument;
	return 0;
}

/**
 * @brief: set the password of the client
 * @param command the command to set the password
 * @return 0 if no error, 1 otherwise
*/
int	Client::setPassword(std::string command)
{
	std::istringstream iss(command);
	std::string cmd;
	std::string pass;
	iss >> cmd;
	getline(iss, pass);
	if (pass.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
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

// ===========COMMANDS CHAN==================

/**
 * @brief: join a channel
 * @param target the channel to join
 * @return 0 if no error, 1 otherwise
*/
int	Client::joinChan(std::string target)
{
	if (_username.empty() || _nickname.empty() || _password == false)
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

	if (argument.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(word, word));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Not enough parameters" << RESET << std::endl;
		return 1;
	}
	if (argument[0] != '#')
	{
		sendMsg(ERR_NOSUCHCHANNEL(word, argument));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such channel" << RESET << std::endl;
		return 1;
	}
	Channel *chan = _server->getChannel(argument);
	if (chan && chan->getLimitUserMod() == true)
	{
		if (static_cast<unsigned long>(chan->getUserLimit()) <= chan->getUserClient().size())
		{
			sendMsg(ERR_CHANNELISFULL(_nickname, argument));
			return 1;
		}
	}
	if (chan && chan->getInviteOnly())
	{
		if (chan->isListed(getNick()))
		{
			chan->removeList(this);
			chan->addClient(this);
			std::vector<Client *> lst = chan->getUserList();
			for (size_t j = 0; j != lst.size(); j++)
			{
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 JOIN " + argument + "\r\n";
				send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
			}
			_alreadyInChannel = true;
			sendMsg(RPL_NAMREPLY(_nickname, argument, chan->getNicks()));
			sendMsg(RPL_ENDOFNAMES(_nickname, argument));
			sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
			chan->getTopic().empty() ? sendMsg(RPL_NOTOPIC(_nickname, argument)) : sendMsg(RPL_TOPIC(_nickname, argument, chan->getTopic()));
			return 0;
		} else {
			sendMsg(ERR_INVITEONLYCHAN(word, argument));
			return 1;
		}
	}
	if ((!chan && !error.empty()) || (!error.empty() && !chan->getPasswd()))
	{
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Too many arguments" << RESET << std::endl;
		return 1;
	}
	if (chan && chan->getPasswd())
	{
		std::string pass = error;
		if (pass == chan->getpass())
		{
			chan->addClient(this);
			std::vector<Client *> lst = chan->getUserList();
			for (size_t j = 0; j != lst.size(); j++)
			{
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 JOIN " + argument + "\r\n";
				send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
			}
			_alreadyInChannel = true;
			sendMsg(RPL_NAMREPLY(_nickname, argument, chan->getNicks()));
			sendMsg(RPL_ENDOFNAMES(_nickname, argument));
			sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
			chan->getTopic().empty() ? sendMsg(RPL_NOTOPIC(_nickname, argument)) : sendMsg(RPL_TOPIC(_nickname, argument, chan->getTopic()));
			return 0;
		} else {
			sendMsg(ERR_BADCHANNELKEY(word, argument));
			return 1;
		}
	}
	if (chan)
	{
		chan->addClient(this);
		std::vector<Client *> lst = chan->getUserList();
		for (size_t j = 0; j != lst.size(); j++)
		{
			std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 JOIN " + argument + "\r\n";
			send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
		}
		_alreadyInChannel = true;
		sendMsg(RPL_NAMREPLY(_nickname, argument, chan->getNicks()));
		sendMsg(RPL_ENDOFNAMES(_nickname, argument));
		sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
		chan->getTopic().empty() ? sendMsg(RPL_NOTOPIC(_nickname, argument)) : sendMsg(RPL_TOPIC(_nickname, argument, chan->getTopic()));
	}
	else
	{
		std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 JOIN " + argument + "\r\n";
		send(_clientFd, msg.c_str(), msg.size(), 0);
		Channel *new_channel = new Channel();
		new_channel->setName(argument);
		new_channel->addClient(this);
		new_channel->addUserMod(this);
		new_channel->setStart(true);
		_server->addChannel(new_channel);
		_perms = true;
		_alreadyInChannel = true;
		sendMsg(RPL_NAMREPLY(_nickname, argument, "@"+_nickname));
		sendMsg(RPL_ENDOFNAMES(_nickname, argument));
		sendMsg(RPL_CHANNELMODEIS(_nickname, argument, "+t"));
		sendMsg(RPL_NOTOPIC(_nickname, argument));
	}
	return 0;
}

/**
 * @brief: set the topic of the channel
 * @param target the channel to set the topic
 * @return 0 if no error, 1 otherwise
*/
int Client::topic(std::string target)
{
	if (_username.empty() || _nickname.empty() || _password == false)
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	std::istringstream iss(target);
	std::string cmd;
	std::string channel;
	std::string arguments;

	iss >> cmd;
	iss >> channel;
	iss >> arguments;

	Channel *chan = _server->getChannel(channel);
	if (!chan)
	{
		sendMsg(ERR_NOSUCHCHANNEL(_nickname, channel));
		return 1;
	}
	if (chan->is_user_mod(this) == false)
	{
		if (chan->getTopicMod() == true)
		{
			std::string msg = ":";
			msg += "localhost 482 ";
			msg += _username;
			msg += " ";
			msg += channel;
			msg += " :You are'nt channel operator";
			msg += "\r\n";
			send(_clientFd, msg.c_str(), msg.size(), 0);
			return 1;
		}
	}
	if (arguments.empty())
	{
		if (chan->getTopic().empty())
		{
			sendMsg(RPL_NOTOPIC(_nickname, channel));
			return 1;
		} else {
			sendMsg(RPL_TOPIC(_nickname, channel, chan->getTopic()));
		}
	} else {
		while (true)
		{
			std::string t;
			iss >> t;
			if (t.empty())
				break ;
			arguments += " " + t;
		}
		chan->setTopic(arguments);
		std::vector<Client *> lst = chan->getUserList();
		for (size_t j = 0; j != lst.size(); j++)
		{
			std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 TOPIC " + channel + " :" + arguments + "\r\n";
			send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
		}
		sendMsg(RPL_TOPIC(_nickname, channel, chan->getTopic()));
	}
	return 0;
}

/**
 * @brief: set the mode of the channel
 * @param target command + target to set the mode
 * @return 0 if no error, 1 otherwise
*/
int	Client::modChan(std::string target)
{
	if (_username.empty() || _nickname.empty() || _password == false)
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	std::istringstream iss(target);
	std::string cmd;
	std::string channel;
	std::string mode;
	std::string argument;
	int add = 0;
	iss >> cmd;
	iss >> channel;
	iss >> mode;
	getline(iss, argument);
	if (mode.empty())
		return 1;
	if (!_server->channelExist(channel))
	{
		sendMsg(ERR_NOSUCHCHANNEL(cmd, channel));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such channel" << RESET << std::endl;
		return 1;
	}
	if (channel.empty())
	{
		sendMsg(ERR_NEEDMOREPARAMS(cmd, cmd));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "channel" << RESET << std::endl;
		return 1;
	}
	if (!_perms)
	{
		sendMsg(ERR_NOPRIVILEGES(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must have permissions to do that" << RESET << std::endl;
		return 1;
	
	}
	if (mode[0] != '+' && mode[0] != '-')
	{
		sendMsg(ERR_UNKNOWNMODE(_nickname, mode));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Unknown mode" << RESET << std::endl;
		return 1;
	}
	else if (mode[0] == '+')
		add = 1;
	mode = mode.substr(1);
	if (handleMode(mode) || mode.size() != 1)
	{
		sendMsg(ERR_UNKNOWNMODE(_nickname, mode));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "Unknown mode" << RESET << std::endl;
		return 1;
	}
	if (add)
	{
		if(handleAddMode(mode, channel, argument))
			return 1;
	}
	else
	{
		if (handleRemoveMode(mode, channel, argument))
			return 1;
	}
	return 0;
}
/**
 * @brief: leave a channel
 * @param target target to leave the channel
 * @return 0 if no error, 1 otherwise
 */
int Client::leaveChan(std::string target)
{
	if (_username.empty() || _nickname.empty() || _password == false)
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	std::string channel;
	for (size_t i = 0; i < target.length(); i++)
	{
		if (target[i] == '#')
		{
			channel = target.substr(i, target.length());
			break ;
		}
	}
	for (size_t i = 0; i < channel.length(); i++)
	{
		if (channel[i] == ' ')
		{
			channel = channel.substr(0, i);
			break ;
		}
	}
	for (size_t i = 0; i < _server->getChannels().size(); i++)
	{
		if (_server->getChannels()[i]->getName() == channel)
		{
			std::vector<Client *> lst = _server->getChannels()[i]->getUserList();
			for (size_t j = 0; j != lst.size(); j++)
			{
				std::string msg = ":";
				msg += _nickname;
				msg += "!";
				msg += _username;
				msg += "@127.0.0.1 PART ";
				msg += channel;
				msg += " :Leaving";
				msg += "\r\n";
				send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
			}
			_server->getChannels()[i]->removeClient(this);
			_server->getChannels()[i]->removeUserMod(this);
			_alreadyInChannel = false;
		}
	}
	return 0; 	
}
/**
 * @brief: kick a user from a channel
 * @param args the arguments to kick a user from a channel
 * @return 0 if no error, 1 otherwise
*/
int Client::kickChan(std::string args)
{
	std::string channel;
	std::string target;
	std::string reason;
	if (_username.empty() || _nickname.empty() || _password == false)
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	for (size_t i = 0; i < args.length(); i++)
	{
		if (args[i] == '#')
		{
			channel = args.substr(i, args.length());
			break ;
		}
	}
	for (size_t i = 0; i < channel.length(); i++)
	{
		if (channel[i] == ' ')
		{
			target = channel.substr(i + 1, channel.length());
			channel = channel.substr(0, i);
			break ;
		}
	}
	for (size_t i = 0; i < target.length(); i++)
	{
		if (target[i] == ':')
		{
			reason = target.substr(i + 1, target.length());
			target = target.substr(0, i - 1);
			break ;
		}
	}
	for (size_t i = 0; i < _server->getChannels().size(); i++)
	{
		if (_server->getChannels()[i]->getName() == channel)
		{
			if (_server->getChannels()[i]->is_user(target) == false)
			{
				sendMsg(ERR_USERNOTINCHANNEL(_nickname, target, channel));
				return 1;
			}
			if (_server->getChannels()[i]->is_user_mod(this) == false)
			{
				std::string msg = ":";
				msg += "localhost 482 ";
				msg += _username;
				msg += " ";
				msg += channel;
				msg += " :You are'nt channel operator";
				msg += "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			} else {
				std::vector<Client *> lst = _server->getChannels()[i]->getUserList();
				for (size_t j = 0; j != lst.size(); j++)
				{
					std::string msg = ":";
					msg += _nickname;
					msg += "!";
					msg += _username;
					msg += "@127.0.0.1 KICK ";
					msg += channel;
					msg += " ";
					msg += target;
					msg += " :";
					msg += reason;
					msg += "\r\n";
					send(lst[j]->get_fd(), msg.c_str(), msg.size(), 0);
				}
				_server->getChannels()[i]->removeClient(target);
				_server->getChannels()[i]->removeUserMod(target);
			}
		}
	}
	return 0;
}

/**
 * @brief: invite a user to a channel
 * @param target the target to invite to a channel
 * @return 0 if no error, 1 otherwise
*/
int				Client::inviteChan(std::string target)
{
	std::istringstream iss(target);
	std::string cmd;
	std::string channel;
	std::string nick;
	iss >> cmd;
	iss >> nick;
	iss >> channel;
	if (_username.empty() || _nickname.empty() || _password == false)
	{
		sendMsg(ERR_NOTREGISTERED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You must be registered first" << RESET << std::endl;
		return 1;
	}
	Channel *chan = _server->getChannel(channel);
	if (!chan)
	{
		sendMsg(ERR_NOSUCHCHANNEL(nick, channel));
		return 1;
	}
	if (chan->is_user(nick))
	{
		sendMsg(ERR_USERONCHANNEL(nick, nick, channel));
		return 1;
	}
	if (!chan->isListed(nick))
	{
		chan->addList(_server->getClient(nick));
		std::string msg = ":localhost 341 " + _nickname +" " + nick + " " + channel + "\r\n";
		send(_clientFd, msg.c_str(), msg.size(), 0);
		for (size_t j = 0; j != chan->getUserList().size(); j++)
		{
			msg = ":";
			msg += _nickname;
			msg += "!";
			msg += _username;
			msg += "@127.0.0.1 INVITE ";
			msg += nick;
			msg += " ";
			msg += channel;
			msg += "\r\n";
			send(chan->getUserList()[j]->get_fd(), msg.c_str(), msg.size(), 0);
		}
	}
	return 0;
}

// ===========COMMAND MSG==================

/**
 * @brief: send a private message to a user or a channel
 * @param input the input to send a private message
 * @return 0 if no error, 1 otherwise
*/
int	Client::prvMsg(std::string input)
{
	if (_username.empty() || _nickname.empty() || _password == false)
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
	msg = msg.substr(1);
	int clientIndex = _server->clientExistNick(target);
	if (target.substr(0, 1) == "#")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == target)
			{
				for (size_t j = 0; j < _server->getChannels()[i]->getUserList().size(); j++)
				{
					if (_server->getChannels()[i]->getUserList()[j]->getNick() != _nickname)
					{
						std::string realsend;
						if (msg[0] == ':')
							realsend += ":" + _nickname + "!" + _username + "@localhost PRIVMSG " + target + " " + msg + "\r\n";
						else
							realsend += ":" + _nickname + "!" + _username + "@localhost PRIVMSG " + target + " :" + msg + "\r\n";
						send(_server->getChannels()[i]->getUserList()[j]->get_fd(), realsend.c_str(), realsend.size(), 0);
					}
				}
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
// ==============================================================================================
// ======================================UTILS CLIENT PART=======================================
// ==============================================================================================

/**
 * @brief: handle the mode of the channel +
 * @param mode the mode to handle
 * @param channel the channel to handle the mode
 * @param argument the argument to handle the mode
 * @return 0 if no error, 1 otherwise
*/
int	Client::handleAddMode(std::string mode, std::string channel, std::string argument)
{
	if (mode == "o")
	{
		if (argument.empty())
		{
			std::string send = "MODE";
			sendMsg(ERR_NEEDMOREPARAMS(send, argument));
			std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "argument" << RESET << std::endl;
			return 1;
		}
		argument = argument.substr(1);
		if (_server->clientExistNick(argument) == -1)
		{
			std::string send = "MODE";
			sendMsg(ERR_NOSUCHNICK(send, argument));
			std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such nick/channel" << RESET << std::endl;
			return 1;
		}
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->addUserMod(_server->getClients()[_server->clientExistNick(argument)]);
				_server->getClients()[_server->clientExistNick(argument)]->setPerms(true);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " +o " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "i")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setInviteOnly(true);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " +i " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "t")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setTopicMod(true);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " +t " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "k")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setHasPasswd(true);
				_server->getChannels()[i]->setPasswd(argument);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " +k :" + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "l")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setLimitUserMod(true);
				_server->getChannels()[i]->setUserLimit(std::atoi(argument.c_str()));
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " +l " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	return 0;
}

/**
 * @brief: handle the mode of the channel -
 * @param mode the mode to handle
 * @param channel the channel to handle the mode
 * @param argument the argument to handle the mode
 * @return 0 if no error, 1 otherwise
*/
int	Client::handleRemoveMode(std::string mode, std::string channel, std::string argument)
{
	if (mode == "o")
		{
			if (argument.empty())
			{
				std::string send = "MODE";
				sendMsg(ERR_NEEDMOREPARAMS(send, argument));
				std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "argument" << RESET << std::endl;
				return 1;
			}
			argument = argument.substr(1);
			if (_server->clientExistNick(argument) == -1)
			{
				std::string send = "MODE";
				sendMsg(ERR_NOSUCHNICK(send, argument));
				std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "No such nick/channel" << RESET << std::endl;
				return 1;
			}
			for (size_t i = 0; i < _server->getChannels().size(); i++)
			{
				if (_server->getChannels()[i]->getName() == channel)
				{
					_server->getChannels()[i]->removeUserMod(_server->getClients()[_server->clientExistNick(argument)]);
					_server->getClients()[_server->clientExistNick(argument)]->setPerms(false);
					std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " -o " + argument + "\r\n";
					send(_clientFd, msg.c_str(), msg.size(), 0);
				}
			}
		}
	else if (mode == "i")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setInviteOnly(false);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " -i " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "t")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setTopicMod(false);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " -t " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "k")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setHasPasswd(false);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " -k " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	else if (mode == "l")
	{
		for (size_t i = 0; i < _server->getChannels().size(); i++)
		{
			if (_server->getChannels()[i]->getName() == channel)
			{
				_server->getChannels()[i]->setLimitUserMod(false);
				std::string msg = ":" + _nickname + "!" + _username + "@127.0.0.1 MODE " + channel + " -l " + argument + "\r\n";
				send(_clientFd, msg.c_str(), msg.size(), 0);
			}
		}
	}
	return 0;
}

/**
 * @brief: set the permissions of the client
 * @param truefalse true of false
 * @return 0 if no error, 1 otherwise
*/
int	Client::setPerms(bool truefalse)
{
	if (_perms == true && truefalse == true)
	{
		sendMsg(ERR_ALREADYREGISTRED(_nickname));
		std::cerr << BLACK << getCurrentTime() << "    " << RED << "Input error: " << "You have already set permissions" << RESET << std::endl;
		return 1;
	}
	if (truefalse == true)
		_perms = true;
	else
		_perms = false;
	return 0;
}

/**
 * @brief: set the registered of the client
 * @param registered true or false
*/
void	Client::setRegistered(bool registered)
{
	if (registered == true)
		_registered = true;
	else
		_registered = false;
}

/**
 * @brief: send a message to the client (handle the \r\n)
 * @param msg the message to send
*/
void	Client::sendMsg(std::string msg)
{
	msg += "\r\n";
	msg.insert(0, ":localhost ");
	send(_clientFd, msg.c_str(), msg.size(), 0);
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

void	Client::setAlreadyInChannel(bool alreadyInChannel)
{
	_alreadyInChannel = alreadyInChannel;
}

bool	Client::getAlreadyInChannel()
{
	return (_alreadyInChannel);
}