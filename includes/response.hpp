/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:58:56 by cpeterea          #+#    #+#             */
/*   Updated: 2024/06/06 15:11:57 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define RPL_WELCOME(n)							"001 " + n + " :Welcome to the ft_irc Network " + n + "!"
# define RPL_YOURHOST(n, serv, ver)				"002 " + n + " :Your host is " + serv + ", running version " + ver
# define RPL_CREATED(n, date)					"003 " + n + " :This server was created " + date
# define RPL_LISTSTART(input)					"321 " + input + " Channel :Users  Name"
# define RPL_LIST(input, chan, users, topic)	"322 " + input + " " + chan + " " + users + " :" + topic
# define RPL_LISTEND(input)						"323 " + input + " :End of /LIST"
# define RPL_CHANNELMODEIS(input, chan, md)		"324 " + input + " " + chan + " " + md
# define RPL_NOTOPIC(input, chan)				"331 " + input + " " + chan + " :No topic is set"
# define RPL_TOPIC(input, chan, topic)			"332 " + input + " " + chan + " :" + topic
# define RPL_INVITING(input, nick, chan)		"341 " + input + " " + nick + " " + chan
# define RPL_NAMREPLY(input, chan, nicks)		"353 " + input + " = " + chan + " :" + nicks
# define RPL_ENDOFNAMES(input, chan)			"366 " + input + " " + chan + " :End of /NAMES list"
#endif