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
		void			removeClient(std::string target);
		Client			*getClient(std::string str);
		std::vector<Client *>	getUserList(void);
		std::vector<Client *>	getUserMod(void);
		void					addUserMod(Client *target);
		void					removeUserMod(Client *target);
		void					removeUserMod(std::string target);
		bool					is_user_mod(Client *target);
		bool					is_user(std::string nick);
		std::string				getNicks(void);

		bool					getStart(void);
		void					setStart(bool start);

		bool					getInviteOnly(void);
		void					setInviteOnly(bool invite_only);

		bool					getTopicMod(void);
		void					setTopicMod(bool topic_mod);

		bool					getPasswd(void);
		void					setPasswd(std::string passwd);
		void					setHasPasswd(bool has_passwd);

		bool					getLimitUserMod(void);
		void					setUserLimit(int limit_user_mod);
		void					setLimitUserMod(bool limit_user_mod);


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
		bool				_start;
};

#endif