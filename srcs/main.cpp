/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeterea <cpeterea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:56:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/17 18:02:50 by cpeterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

/**
 * @brief: main function
 * 
 * @param ac the number of arguments
 * @param av the arguments passed to the program (port and password)
 * 
*/
int main(int ac, char **av)
{
	if (handle_input(ac, av) == 1)
		return (1);
	
	Server server(atoi(av[1]), av[2]);
	try
	{
		signal(SIGINT, Server::signalHandler); // CTRL + C (add signalHandler to handle the signal)
		signal(SIGQUIT, Server::signalHandler); // CTRL + \ (add signalHandler to handle the signal)

		server.start(); 
	}
	catch(const std::exception& e)
	{
		server.closeFds();
		server.freeClients();
		std::cerr << e.what() << '\n';
	}
	
	std::cout << BLACK << getCurrentTime() << "    " << YELLOW << "Server is closing" << RESET << std::endl;
	return (0);
}
