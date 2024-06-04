/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:40:30 by cpeterea          #+#    #+#             */
/*   Updated: 2024/06/04 12:38:35 by cpeterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"


class Channel
{
	public:
        Channel();
        Channel(bool pv);
        ~Channel();
        
        std::vector<Client> getUserList(void);
        int                 getID(void);
        std::string         getTopic(void);
        std::string         getPasswd(void);
        bool                getPrivate(void);
        int                 getUserLimit(void);
        bool                hasPass(void);
        bool                isInviteOnly(void);
        bool                isTopicMod(void);
        bool                isLimitUserMod(void);
        

	private:
        std::vector<Client> _user_list;
        int     _id;
        std::string     _topic;
        bool            _is_private;
        int             user_limit;
        std::string     _passwd;
        bool            _has_passwd;
        bool            _invite_only;
        bool            _topic_mod;
        bool            _limit_user_mod;
};

#endif