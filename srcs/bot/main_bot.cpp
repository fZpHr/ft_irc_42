/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bot.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:27:56 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/11 17:00:11 by ben              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "TicTacToe.hpp"

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

// #include <sstream>

// int main(void) {
// 	TicTacToe game;
// 	std::string move;

// 	std::cout << game.getBoard() << std::endl;
// 	int i = 0;
// 	while (i < 5) {
// 		std::cout << "Enter move [row-col]: ";
// 		std::cin >> move;
// 		std::cout << std::endl << std::endl;
// 		std::cout << game.play(move, 'X') << std::endl;
// 		i++;
// 	}
// }
