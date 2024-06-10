/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 19:46:28 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/10 20:04:36 by bberkrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TicTacToe.hpp"

// ============ Constructor ============

TicTacToe::TicTacToe() {
	resetBoard();
}

TicTacToe::TicTacToe(const TicTacToe &src) {
	*this = src;
}

// ============ Destructor ============

TicTacToe::~TicTacToe() {
}

// ============ Operator ============

TicTacToe &TicTacToe::operator=(const TicTacToe &rhs) {
	if (this != &rhs) {
	}
	return (*this);
}

// ============ Member Functions ============

bool TicTacToe::makeMove(int row, int col, char player) {
	if (board[row][col] == '0') {
		board[row][col] = player;
		return (true);
	}
	return (false);
}

bool TicTacToe::checkWin(char player) {
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
			return (true);
		}
		if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
			return (true);
		}
	}
	if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
		return (true);
	}
	if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
		return (true);
	}
	return (false);
}

void	TicTacToe::resetBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = '0';
		}
	}
}

std::string TicTacToe::play(int row, int col, char player) {
	std::string state;
	
	state = getBoard() + '\n';
	if (makeMove(row, col, player)) {
		state = getBoard() + '\n';
		if (checkWin(player)) {
			state += "Player " + std::string(1, player) + " wins!";
		} else {
			state += "Player " + std::string(1, player) + " played at (" + std::to_string(row) + ", " + std::to_string(col) + ")";
		}
	} else {
		state += "Invalid move! try again.";
	}
	
	return state;
}


// ============ Getters ============

std::string TicTacToe::getBoard() {
    std::string board_str = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board_str += board[i][j];
        }
		board_str += "\n";
    }
    return board_str;
}



// ============ Setters ============