/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:56:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/05/27 21:28:01 by hbelle           ###   ########.fr       */
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
	
	Server server;
	try
	{
		signal(SIGINT, Server::signalHandler); // CTRL + C (add signalHandler to handle the signal)
		signal(SIGQUIT, Server::signalHandler); // CTRL + \ (add signalHandler to handle the signal)
		server.start(); 
	}
	catch(const std::exception& e)
	{
		server.closeFds();
		std::cerr << e.what() << '\n';
	}
	
	std::cout << "Server is closing" << std::endl;
	return (0);
}