/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:19:54 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/05 17:05:06 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERROR_HPP
# define ERROR_HPP

# define RPL_LISTSTART(input)					"321 " + input + " Channel :Users  Name"
# define RPL_LIST(input, chan, users, topic)	"322 " + input + " " + chan + " " + users + " :" + topic
# define RPL_LISTEND(input)						"323 " + input + " :End of /LIST"
# define RPL_CHANNELMODEIS(input, chan, md)		"324 " + input + " " + chan + " " + md
# define RPL_NOTOPIC(input, chan)				"331 " + input + " " + chan + " :No topic is set"
# define RPL_TOPIC(input, chan, topic)			"332 " + input + " " + chan + " :" + topic
# define RPL_INVITING(input, nick, chan)		"341 " + input + " " + nick + " " + chan
# define RPL_NAMREPLY(input, chan, nicks)		"353 " + input + " = " + chan + " :" + nicks
# define RPL_ENDOFNAMES(input, chan)			"366 " + input + " " + chan + " :End of /NAMES list"

# define ERR_NOSUCHNICK(input, nick)			"401 " + input + " " + nick + " :No such nick/channel"
# define ERR_NOSUCHCHANNEL(input, chan)			"403 " + input + " " + chan + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(input, chan)		"404 " + input + " " + chan + " :Cannot send to channel"
# define ERR_NOORIGIN(input)					"409 " + input + " :No origin specified"
# define ERR_NORECIPIENT(input)					"411 " + input + " :No recipient given"
# define ERR_NOTEXTTOSEND(input)				"412 " + input + " :No text to send"
# define ERR_UNKNOWNCOMMAND(input, cmd)			"421 " + input + " " + cmd + " :Unknown command"
# define ERR_NONICKNAMEGIVEN(input)				"431 " + input + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(input, nick)		"432 " + input + " " + nick + " :Erroneous nickname"
# define ERR_NICKNAMEINUSE(input, nick)			"433 " + input + " " + nick + " :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(input, nick, chn)	"441 " + input + " " + nick + " " + chn + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(input, chan)			"442 " + input + " " + chan + " :You're not on that channel"
# define ERR_USERONCHANNEL(input, nick, chan)	"443 " + input + " " + nick + " " + chan + " :is already on channel"
# define ERR_NEEDMOREPARAMS(input, cmd)			"461 " + input + " " + cmd + " :Not enough parameters"
# define ERR_ALREADYREGISTRED(input)			"462 " + input + " :Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH(input)				"464 " + input + " :Password incorrect"
# define ERR_CHANNELISFULL(input, chan)			"471 " + input + " " + chan + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(input, mode)			"472 " + input + " " + mode + " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(input, chan)		"473 " + input + " " + chan + " :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY(input, chan)			"475 " + input + " " + chan + " :Cannot join channel (+k)"
# define ERR_CHANOPRIVSNEEDED(input, chan)		"482 " + input + " " + chan + " :You're not channel operator"

#endif