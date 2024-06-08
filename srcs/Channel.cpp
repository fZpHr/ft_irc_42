#include "Channel.hpp"

Channel::Channel()
{
	_id = 0;
	_topic = "";
	_is_private = false;
	_user_limit = 0;
	_passwd = "";
	_has_passwd = false;
	_invite_only = false;
	_topic_mod = false;
	_limit_user_mod = false;
}

Channel::Channel(bool pv){
	_id = 0;
	_topic = "";
	_is_private = pv;
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



std::string				Channel::getNicks(void)
{
	std::string msg = "";
	for (std::vector<Client *>::iterator itr = _userList.begin(); itr != _userList.end(); ++itr)
	{
		msg += (*itr)->getNick();
	}
	return (msg);
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

