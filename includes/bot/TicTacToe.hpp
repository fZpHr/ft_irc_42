/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 19:38:12 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/10 20:01:09 by bberkrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TICTACTOE_HPP
# define TICTACTOE_HPP

# include <iostream>
# include <string>

class TicTacToe
{
	public:
		TicTacToe();
		TicTacToe(const TicTacToe &src);
		~TicTacToe();

		TicTacToe &operator=(const TicTacToe &rhs);

		std::string play(int row, int col, char player);
		
		bool makeMove(int row, int col, char player);
		bool checkWin(char player);                  
		void resetBoard();
		std::string getBoard();
	private:
		char board[3][3];
};

#endif