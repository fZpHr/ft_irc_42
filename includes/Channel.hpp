/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:49:56 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 16:49:58 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Channel
{
	public:
		Channel();
		~Channel();

		void					setName(std::string name);
		void					setTopic(std::string topic);
		std::string				getName();
		void					setInviteOnly(bool invite_only);
		void					setTopicMod(bool topic_mod);
		void					setPasswd(std::string passwd);
		void					setHasPasswd(bool has_passwd);
		void					setUserLimit(int limit_user_mod);
		void					setStart(bool start);
		void					setLimitUserMod(bool limit_user_mod);
		
		Client					*getClient(std::string str);
		std::string				getTopic();
		std::vector<Client *>	getUserList(void);
		std::vector<Client *>	getUserMod(void);
		std::vector<Client *>	getUserClient(void);
		bool					getInviteOnly(void);
		bool					getTopicMod(void);
		bool					getPasswd(void);
		std::string				getpass(void);
		bool					getStart(void);
		bool					getLimitUserMod(void);
		int						getUserLimit();
		std::vector<Client *>	getList(void);
		std::string				getNicks(void);

		void					addClient(Client *client);
		void					removeClient(Client *client);
		void					removeClient(std::string target);
		void					addUserMod(Client *target);
		void					removeUserMod(Client *target);
		void					removeUserMod(std::string target);
		
		bool					is_user_mod(Client *target);
		bool					is_user(std::string nick);


		bool					isListed(std::string nick);
		void					addList(Client *client);
		void					removeList(Client *client);

	private:
		Channel(Channel const &src);
		Channel &operator=(Channel const &rhs);
		std::string			_name;
		std::vector<Client *> _userList;
		std::vector<Client *> _mod;
		std::vector<Client *> _invited;
		int					_id;
		std::string			_topic;
		int					_user_limit;
		std::string			_passwd;
		bool				_has_passwd;
		bool				_invite_only;
		bool				_topic_mod;
		bool				_limit_user_mod;
		bool				_start;
};

#endif