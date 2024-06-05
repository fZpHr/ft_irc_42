/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:28:53 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/05 22:13:16 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.hpp"

int handle_input(int ac, char **av)
{

	if (ac != 3)
	{
		std::cerr << BRED << "Error: " << RED << "invalid number of arguments" << RESET << std::endl;
		std::cerr << BGREEN << "Usage: " << GREEN ITALIC << av[0]  << " \"the listening port\"" << " " << "\"the connection password\"" << RESET << std::endl;
		return (1);
	}
	int port = atoi(av[1]);
	if (port < 1024 || port > 65535)
	{
		std::cerr << BRED << "Error: invalid port range: " << RED ITALIC << av[1] << RESET << std::endl;
		return (1);
	}
	return (0);
}