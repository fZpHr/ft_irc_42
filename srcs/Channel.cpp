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

// std::vector<Client>	Channel::getUserList(void){
// 	return _user_list;
// }

// int                 Channel::getID(void)
// {
// 	return _id;
// }

// std::string         Channel::getTopic(void){
// 	return _topic;

// }

// std::string         Channel::getPasswd(void){
// 	return _passwd;
// }

// bool                Channel::getPrivate(void){
// 	return _is_private;
// }

// int                 Channel::getUserLimit(void){
// 	return _user_limit;
// }

// bool                Channel::hasPass(void){
// 	return _has_passwd;
// }

// bool                Channel::isInviteOnly(void){
// 	return _invite_only;
// }
// bool                Channel::isTopicMod(void){
// 	return _topic_mod;
// }

// bool                Channel::isLimitUserMod(void){
// 	return _limit_user_mod;
// }

// void                Channel::setTopic(std::string topic){
// 	_topic = topic;
// }

// void                Channel::setUserLimit(int limit){
// 	_user_limit = limit;
// }

// void                Channel::setPrivate(bool is_private){
// 	_is_private = is_private;
// }

// void                Channel::setPasswd(std::string passwd){
// 	_passwd = passwd;
// }

