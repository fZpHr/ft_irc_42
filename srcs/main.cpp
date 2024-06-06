/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:56:02 by hbelle            #+#    #+#             */
/*   Updated: 2024/06/06 14:59:54 by hbelle           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
// #include <thread> 
// #include <atomic>
// #include <mutex>
// std::atomic<bool> running(true);
// std::mutex cout_mutex;

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
	if (DEBUG)
	{
		// std::thread serverStateThread([&server]()
		// {
		// 	while (running)
		// 	{
		// 		cout_mutex.lock();
		// 		std::cout << "\033[2J\033[H";
		// 		std::cout << YELLOW << "-----------Server state-----------" << RESET << std::endl;
		// 		server.printState();
		// 		cout_mutex.unlock();
		// 		std::this_thread::sleep_for(std::chrono::seconds(1));
		// 	}
		// });
		// serverStateThread.detach();
	}
	try
	{
		signal(SIGINT, Server::signalHandler); // CTRL + C (add signalHandler to handle the signal)
		signal(SIGQUIT, Server::signalHandler); // CTRL + \ (add signalHandler to handle the signal)
		Channel* channel = new Channel();

		channel->setName("General");
		channel->setTopic("General Discussion");
		// channel->setUserLimit(50);
		// channel->setPrivate(false);
		server.addChannel(channel);
		server.start(); 
	}
	catch(const std::exception& e)
	{
		server.closeFds();
		std::cerr << e.what() << '\n';
	}
	
	std::cout << YELLOW << "Server is closing" << RESET << std::endl;
	return (0);
}