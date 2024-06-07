/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:38:37 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/07 19:58:51 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

#define REGEXMESSAGE "^:([^ ]*) ?([^ ]*) ?(.*\r\n$)"
#define REGEXPREFIX "^([^!@]*)|([^!@]*)(!([^@]*))?@(.*)$"
#define REGEXCOMMAND "^[a-zA-Z]*|[0-9]{3}$"
#define REGEXPARAMS "^(([^:]* )*)?:?(.*)$"
#define REGEXTARGET "^([^,]*),?(.*)$"
#define REGEXMSGTO "^([^,]*)|([^@%]*"
#define REGEXCHANNEL "^([#+!&])([^:]*)(:(.*))?$"
#define REGEXSERVERNAME "^(.*)$"
#define REGEXHOST "^([^:]*)|(.*$)"
#define REGEXHOSTNAME "^([a-zA-Z0-9-]*)(\\.([a-zA-Z0-9-]*))*$"
#define REGEXHOSTADDR "^(([0-9]{1,3}\\.){3}[0-9]{1,3})|(([0-9a-fA-F]*:){7}[0-9a-fA-F]*)|0:0:0:0:0:(0|FFFF):(([0-9]{1,3}\\.){3}[0-9]{1,3})$"
#define REGEXNICKNAME "^[a-zA-Z][a-zA-Z0-9_.-]{0,8}$"
#define REGEXTARGETMASK "^[\\$#](.*)$"
#define REGEXCHANSTRING "^[^\x00\x07\x0A\x0D ,:]*$"
#define REGEXCHANNELID "^[A-Z0-9]{5}$"
#define REGEXUSER "^[^\x00\x0A\x0D @]*$"
#define REGEXKEY "^[^\x00\x0A\x0D\x0C\x09\x20]{1,23}$"
#define REGEXLETTER "^[a-zA-Z]$"
#define REGEXDIGIT "^[0-9]$"
#define REGEXHEXDIGIT "^[0-9A-F]$"
#define REGEXSPECIAL "^[\\[\\]\\`_^{|}]$"