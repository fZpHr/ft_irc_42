/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 19:38:12 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/11 21:23:09 by ben              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TICTACTOE_HPP
# define TICTACTOE_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <limits.h>
#include <sstream>


class TicTacToe
{
	public:
		// ========== Constructors ========== //
		TicTacToe();
		TicTacToe(const TicTacToe &src);
		TicTacToe &operator=(const TicTacToe &rhs);

		// ========== Destructor ========== //
		~TicTacToe();

		// ========== Member functions ========== //
		std::string			play(std::string move, char player);
		std::string			getBoard() const;
		void				resetBoard();
		
	private:
		// ========== Data PART ========== //
		char board[3][3];
		std::pair<int, int> findBestMove();

		// ========== Game PART ========== //
		bool 				makeMove(int row, int col, char player);
		std::pair<int, int> getCorrMove(const std::string& move) const;
		
		bool 				checkWin(char player) const;
		bool 				isNull() const;
		bool 				checkWinRows(char player) const;
		bool 				checkWinColumns(char player) const;
		bool 				checkWinDiagonals(char player) const;
		
		std::string 		handlePlayerWin(char player) const;
		std::string 		handleAIMove(char player);
		std::string 		convertToString(int num) const;

		// ========== IA PART ========== //
		int 				evaluate(char board[3][3]) const;
		int 				evaluateLines(char board[3][3]) const;
		int 				evaluateColumns(char board[3][3]) const;
		int 				evaluateDiagonals(char board[3][3]) const;

		int 				minimax(char board[3][3], int depth, bool isMax);
		bool				areMovesLeft(char board[3][3]) const;
		int 				getMaxScore(char board[3][3], int depth);
		int 				getMinScore(char board[3][3], int depth);

		bool				isValideMove(std::string move) const;
};

#endif