/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:28:53 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/27 21:26:53 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.hpp"

std::string password = "slt";
std::string port = "6667";

int handle_input(int ac, char **av)
{
	int error = 0;

	if (ac != 3)
	{
		std::cerr << BRED << "Error: " << RED << "invalid number of arguments" << RESET << std::endl;
		std::cerr << BGREEN << "Usage: " << GREEN ITALIC << av[0]  << " \"the listening port\"" << " " << "\"the connection password\"" << RESET << std::endl;
		return (1);
	}
	if (av[1] != port)
	{
		std::cerr << BRED << "Error: invalid port: " << RED ITALIC << av[1] << RESET << std::endl;
		error++;
	}
	if (av[2] != password)
	{
		std::cerr << BRED << "Error: invalid password: " << RED ITALIC << av[2] << RESET << std::endl;
		error++;
	}
	if (error > 0)
		return (1);
	// std::cout << "Port: " << av[1] << std::endl;
	// std::cout << "Password: " << av[2] << std::endl;
	return (0);
}