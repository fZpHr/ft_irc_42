/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bot.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:27:56 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/10 19:30:26 by bberkrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ./bot <port> <password>" << std::endl;
        return (1);
    }
    Bot bot;

    try {
        const char* address = "127.0.0.1";
        const char* port = argv[1];
        const char* password = argv[2];

        bot.connect(address, port, password);
		std::cout << "Connected to " << address << ":" << port << std::endl;
		sleep(1);
        bot.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}
