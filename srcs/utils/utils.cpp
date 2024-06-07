/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 22:00:11 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/07 22:00:34 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

#include <ctime>
#include <string>

std::string getCurrentTime()
{
	std::time_t result = std::time(0);
	std::string timeStr(ctime(&result));
	return timeStr.substr(0, timeStr.size()-1);
}