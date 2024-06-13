/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 22:00:11 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/13 18:29:03 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

std::string getCurrentTime()
{
	std::time_t result = std::time(0);
	std::string timeStr(ctime(&result));
	return timeStr.substr(0, timeStr.size()-1);
}

int	handleMode(std::string mode)
{
	std::string valid = "itkol";
	if (mode.find_first_not_of(valid) != std::string::npos)
		return 1;
	return 0;
}