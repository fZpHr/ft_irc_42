/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bot.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:27:56 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/10 20:09:33 by bberkrou         ###   ########.fr       */
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

// int main(void) {
// 	TicTacToe game;

// 	std::cout << game.play(0, 0, 'X') << std::endl;
// 	std::cout << game.play(0, 0, 'X') << std::endl;
// 	std::cout << game.play(0, 1, 'X') << std::endl;
// 	game.resetBoard();
// 	std::cout << game.play(1, 0, '#') << std::endl;
// 	std::cout << game.play(0, 2, 'X') << std::endl;

	
// }
