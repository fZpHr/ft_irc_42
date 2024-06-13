#include "Channel.hpp"

Channel::Channel()
{
	_id = 0;
	_topic = "";
	_user_limit = 0;
	_passwd = "";
	_has_passwd = false;
	_invite_only = false;
	_topic_mod = false;
	_limit_user_mod = false;
	_start = false;
}

Channel::~Channel()
{

}

void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string	Channel::getName()
{
	return (_name);
}

std::string	Channel::getTopic()
{
	return (_topic);
}

void			Channel::addClient(Client* client)
{
	_userList.push_back(client);
}

std::vector<Client *>	Channel::getUserList(void)
{
	return _userList;
}

std::vector<Client *>	Channel::getUserMod(void)
{
	return _mod;
}

void					Channel::addUserMod(Client *target)
{
	_mod.push_back(target);
}

void			Channel::removeClient(Client *client)
{
	for (std::vector<Client *>::iterator itr = _userList.begin(); itr != _userList.end(); ++itr)
	{
		if ((*itr)->getUser() == client->getUser())
		{
			_userList.erase(itr);
			return ;
		}
	}
}

void			Channel::removeUserMod(Client *client)
{
	for (std::vector<Client *>::iterator itr = _mod.begin(); itr != _mod.end(); ++itr)
	{
		if ((*itr)->getUser() == client->getUser())
		{
			_mod.erase(itr);
			return ;
		}
	}
}

std::string				Channel::getNicks(void)
{
	std::string list = "";
	for (size_t i = 0; i < _userList.size(); i++)
	{
		if (i == 0)
			list += "@";
		list += _userList[i]->getNick();
		list += " ";
	}
	return (list);
}


bool					Channel::is_user_mod(Client *target)
{
	for (std::vector<Client *>::iterator itr = _mod.begin(); itr != _mod.end(); ++itr)
	{
		if ((*itr)->getNick() == target->getNick())
			return (true);
	}
	return (false);
}

bool					Channel::is_user(std::string nick)
{
	for (std::vector<Client *>::iterator itr = _userList.begin(); itr != _userList.end(); ++itr)
	{
		if ((*itr)->getNick() == nick)
			return (true);
	}
	return (false);
}


bool					Channel::getStart(void)
{
	return (_start);
}

void					Channel::setStart(bool start)
{
	if (start == false)
		_start = false;
	else
		_start = true;
}

bool					Channel::getInviteOnly(void)
{
	return (_invite_only);
}

void					Channel::setInviteOnly(bool invite_only)
{
	_invite_only = invite_only;
}

bool					Channel::getTopicMod(void)
{
	return (_topic_mod);
}

void					Channel::setTopicMod(bool topic_mod)
{
	_topic_mod = topic_mod;
}

bool					Channel::getPasswd(void)
{
	return (_has_passwd);
}

void					Channel::setPasswd(std::string passwd)
{
	_passwd = passwd;
}

void					Channel::setHasPasswd(bool has_passwd)
{
	_has_passwd = has_passwd;
}

bool					Channel::getLimitUserMod(void)
{
	return (_limit_user_mod);
}

void					Channel::setLimitUserMod(bool limit_user_mod)
{
	_limit_user_mod = limit_user_mod;
}

void					Channel::setUserLimit(int limit_user_mod)
{
	_user_limit = limit_user_mod;
}