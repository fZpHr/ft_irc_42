/* ************************************************************************** */
/*	 */
/*	 :::	 ::::::::   */
/*   Channel.hpp	:+:	 :+:    :+:   */
/*	  +:+ +:+	    +:+	*/
/*   By: hbelle <hbelle@student.42.fr>	    +#+  +:+	  +#+	   */
/*	   +#+#+#+#+#+   +#+	 */
/*   Created: 2024/06/03 17:40:30 by cpeterea	#+#    #+#	   */
/*   Updated: 2024/06/04 16:15:58 by hbelle	 ###   ########.fr	  */
/*	 */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Channel
{
	public:
		Channel();
		~Channel();

		void			setName(std::string name);
		void			setTopic(std::string topic);
		std::string		getName();
		std::string		getTopic();
		void			addClient(Client *client);
		void			removeClient(Client *client);
		std::vector<Client *>	getUserList(void);
		std::vector<Client *>	getUserMod(void);
		void					addUserMod(Client *target);
		void					removeUserMod(Client *target);
		std::string				getNicks(void);
		// void				setUserLimit(int limit);
		// void				setPrivate(bool is_private);
		// void				setPasswd(std::string passwd);
		// void				setHasPasswd(bool has_passwd);
		// void				setInviteOnly(bool invite_only);
		// void				setTopicMod(bool topic_mod);
		// std::vector<Client>	getUserList(void);
		// int					getID(void);
		// std::string			getTopic(void);
		// std::string			getPasswd(void);
		// bool				getPrivate(void);
		// int					getUserLimit(void);
		// bool				hasPass(void);
		// bool				isInviteOnly(void);
		// bool				isTopicMod(void);
		// bool				isLimitUserMod(void);

	private:
		Channel(Channel const &src);
		Channel &operator=(Channel const &rhs);
		std::string			_name;
		std::vector<Client *> _userList;
		std::vector<Client *> _mod;
		int					_id;
		std::string			_topic;
		int					_user_limit;
		std::string			_passwd;
		bool				_has_passwd;
		bool				_invite_only;
		bool				_topic_mod;
		bool				_limit_user_mod;
};

#endif