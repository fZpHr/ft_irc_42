/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TicTacToe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bberkrou <bberkrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 19:46:28 by bberkrou          #+#    #+#             */
/*   Updated: 2024/06/12 21:58:16 by bberkrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot/TicTacToe.hpp"

// ===================================================================================================
// ======================================== Utils Class PART =========================================
// ===================================================================================================

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
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = rhs.board[i][j];
            }
        }
	}
	return (*this);
}

// ===================================================================================================
// ======================================= Game function PART ========================================
// ===================================================================================================

/**
 * @brief Makes a move on the board for the given player if the move is valid.
 * 
 * This function checks if the specified row and column are within bounds and if the
 * cell is empty before placing the player's marker. It returns true if the move is
 * successful and false otherwise.
 * 
 * @param row The row index where the player wants to place their marker.
 * @param col The column index where the player wants to place their marker.
 * @param player The character representing the player ('X' or 'O').
 * @return true if the move is successfully made, false if the move is invalid or out of bounds.
 */
bool TicTacToe::makeMove(int row, int col, char player) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return (false);

    if (board[row][col] == ' ') {
        board[row][col] = player;
        return (true);
    }
    return (false);
}

/**
 * @brief Resets the game board to its initial empty state.
 * 
 * This function iterates over all cells of the board and sets each one to a space character,
 * representing an empty cell. It ensures that the indices do not exceed the board dimensions.
 */
void	TicTacToe::resetBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = ' ';
		}
	}
}

/**
 * @brief Checks if the game board is completely filled with no empty spaces.
 *
 * This function iterates over all cells of the board to check if any empty space exists.
 * If no empty space is found, it returns true indicating the game is null (draw).
 *
 * @return True if the game is null (draw), false otherwise.
 */
bool    TicTacToe::isNull() const {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return (false);
		}
	}
    return (true);
}

/**
 * @brief Checks if a move string is valid.
 *
 * This function verifies if the move string is of length 3, the first and third characters
 * are digits between '0' and '2', and the second character is a dash ('-').
 *
 * @param move The move string to validate.
 * @return True if the move is valid, false otherwise.
 */
bool    TicTacToe::isValideMove(std::string move) const {
    if (move.length() != 3)
        return (false);
    if (!std::isdigit(move[0]) || move[0] < '0' || move[0] > '2')
        return (false);
    if (move[1] != '-')
        return (false);
    if (!std::isdigit(move[2]) || move[2] < '0' || move[2] > '2')
        return (false);
    return (true);
}

// ===================================================================================================
// ======================================= Game Check Win PART =======================================
// ===================================================================================================

/**
 * @brief Checks if the given player has won the game.
 * 
 * This function checks the rows, columns, and diagonals to see if the given player has a winning configuration.
 * 
 * @param player The character representing the player ('X' or 'O').
 * @return true if the player has won, false otherwise.
 */
bool TicTacToe::checkWin(char player) const {
    return (checkWinRows(player) || checkWinColumns(player) || checkWinDiagonals(player));
}

/**
 * @brief Checks if the given player has won by completing any row.
 * 
 * This function iterates over all rows and checks if any row is completely occupied by the given player.
 * 
 * @param player The character representing the player ('X' or 'O').
 * @return true if the player has a complete row, false otherwise.
 */
bool TicTacToe::checkWinRows(char player) const {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == player && board[row][1] == player && board[row][2] == player)
            return (true);
    }
    return (false);
}

/**
 * @brief Checks if the given player has won by completing any column.
 * 
 * This function iterates over all columns and checks if any column is completely occupied by the given player.
 * 
 * @param player The character representing the player ('X' or 'O').
 * @return true if the player has a complete column, false otherwise.
 */
bool TicTacToe::checkWinColumns(char player) const {
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == player && board[1][col] == player && board[2][col] == player)
            return (true);
    }
    return (false);
}

/**
 * @brief Checks if the given player has won by completing any diagonal.
 * 
 * This function checks both diagonals to see if either is completely occupied by the given player.
 * 
 * @param player The character representing the player ('X' or 'O').
 * @return true if the player has a complete diagonal, false otherwise.
 */
bool TicTacToe::checkWinDiagonals(char player) const {
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return (true);
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return (true);
    return (false);
}

// ===================================================================================================
// ======================================== Game action PART =========================================
// ===================================================================================================

/**
 * @brief Plays a move in the TicTacToe game for a given player.
 *
 * This function allows a player to make a move and updates the game state accordingly.
 * It first checks if the move format is valid, then if the move is legal (the cell is not already taken).
 * The game state is updated, and it checks for winning conditions, a draw, or switches to the AI's move.
 *
 * @param move The move to be played in the format "row-col".
 * @param player The character representing the player ('X' or 'O').
 * @return A string representing the current state of the game board and any messages regarding the move.
 */
std::string TicTacToe::play(std::string move, char player) {
    std::string state = "";
    std::pair<int, int> move_int;
    
    if (!isValideMove(move))
        return ("===== Invalide format move =====\n" + getBoard() + "\r\n");
    
    move_int = getCorrMove(move);
    int row = move_int.first;
    int col = move_int.second;

    if (checkWin('X'))
        return ("===== Player " + std::string(1, player) + " wins! =====\n" + getBoard());
    else if (checkWin('O'))
        return ("===== AI wins! =====\n" + getBoard());
    else if (isNull())
        return ( "===== Match NULL =====\n" + getBoard());

    if (makeMove(row, col, player)) {
        if (checkWin(player))
            state += handlePlayerWin(player);
        else if (isNull())
            state += "===== Match NULL =====\n";
        else
            state += handleAIMove();
    } else
        state += "===== Invalid move! try again. =====\n";
    state += getBoard() + '\n';
    return (state);
}

/**
 * @brief Handles the scenario where the player wins.
 * 
 * This function returns a string indicating that the player has won.
 * 
 * @param player The character representing the player who won.
 * @return A string indicating the player's victory.
 */
std::string TicTacToe::handlePlayerWin(char player) const {
    return ("===== Player " + std::string(1, player) + " wins! =====\n");
}

/**
 * @brief Handles the AI's move after the player's move.
 * 
 * This function determines the best move for the AI, makes the move, and checks for a win.
 * 
 * @param player The character representing the player who made the initial move.
 * @return A string representing the state of the game after the AI's move.
 */
std::string TicTacToe::handleAIMove( void ) {
    std::string state = "";
    std::pair<int, int> bestMove = findBestMove();
    int move_x = bestMove.first;
    int move_y = bestMove.second;

    if (makeMove(move_x, move_y, 'O')) {
        if (checkWin('O'))
            state += "===== AI wins! =====\n";
        else
            state += "===== AI played (" + convertToString(move_x) + ", " + convertToString(move_y) + ") =====\n";
    } else
        state += "Invalid move IA at (" + convertToString(move_x) + ", " + convertToString(move_y) + ")\n";
    return (state);
}

std::string TicTacToe::convertToString(int num) const {
    return (std::string(1, '0' + num));
}

// ===================================================================================================
// ============================================= IA PART =============================================
// ===================================================================================================

/**
 * @brief Evaluates the board to determine if there is a winner.
 * 
 * This function calls other helper functions to check the rows, columns, and diagonals
 * to determine if there is a winning configuration on the board.
 * 
 * @param b The current state of the game board.
 * @return 10 if 'O' wins, -10 if 'X' wins, 0 otherwise.
 */
int TicTacToe::evaluate(char b[3][3]) const {
    int score = evaluateLines(b);
    if (score != 0)
		return (score);

    score = evaluateColumns(b);
    if (score != 0)
		return (score);

    score = evaluateDiagonals(b);	
    return (score);
}

/**
 * @brief Evaluates the rows of the board to check for a winning configuration.
 * 
 * This function checks each row to see if there is a winning configuration for either player.
 * 
 * @param b The current state of the game board.
 * @return 10 if 'O' wins in any row, -10 if 'X' wins in any row, 0 otherwise.
 */
int TicTacToe::evaluateLines(char b[3][3]) const {
    for (int row = 0; row < 3; row++) {
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
            if (b[row][0] == 'O')
                return (+15);
            else if (b[row][0] == 'X')
                return (-10);
        }
    }
    return (0);
}

/**
 * @brief Evaluates the columns of the board to check for a winning configuration.
 * 
 * This function checks each column to see if there is a winning configuration for either player.
 * 
 * @param b The current state of the game board.
 * @return 10 if 'O' wins in any column, -10 if 'X' wins in any column, 0 otherwise.
 */
int TicTacToe::evaluateColumns(char b[3][3]) const {
    for (int col = 0; col < 3; col++) {
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
            if (b[0][col] == 'O')
                return (+15);
            else if (b[0][col] == 'X')
                return (-10);
        }
    }
    return (0);
}

/**
 * @brief Evaluates the diagonals of the board to check for a winning configuration.
 * 
 * This function checks the two diagonals to see if there is a winning configuration for either player.
 * 
 * @param b The current state of the game board.
 * @return 10 if 'O' wins in any diagonal, -10 if 'X' wins in any diagonal, 0 otherwise.
 */
int TicTacToe::evaluateDiagonals(char b[3][3]) const {
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
        if (b[0][0] == 'O')
            return (+15);
        else if (b[0][0] == 'X')
            return (-10);
    }
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
        if (b[0][2] == 'O')
            return (+15);
        else if (b[0][2] == 'X')
            return (-10);
    }
    return (0);
}

/**
 * @brief Implements the Minimax algorithm to determine the best score for the current player.
 * 
 * This function evaluates the board and returns the best score that can be achieved by the AI
 * or the human player based on the current board state.
 * 
 * @param board The current state of the game board.
 * @param depth The current depth of the recursion.
 * @param isMax Boolean indicating whether it is the maximizer's turn (true for AI, false for human).
 * @return The best score that can be achieved from the current board state.
 */
int TicTacToe::minimax(char board[3][3], int depth, bool isMax) {
    int score = evaluate(board);
    if (score == 10 || score == -10)
        return (score);

    if (!areMovesLeft(board))
        return (0);

    if (isMax)
        return (getMaxScore(board, depth));
    else
        return (getMinScore(board, depth));
}

/**
 * @brief Checks if there are any moves left on the board.
 * 
 * This function scans the board to see if there are any empty spaces remaining.
 * 
 * @param board The current state of the game board.
 * @return true if there are moves left, false otherwise.
 */
bool TicTacToe::areMovesLeft(char board[3][3]) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return (true);
        }
    }
    return (false);
}

/**
 * @brief Determines the best score for the maximizer (AI) player.
 * 
 * This function iterates over all possible moves, applies the Minimax algorithm recursively,
 * and returns the highest score that can be achieved by the AI player.
 * 
 * @param board The current state of the game board.
 * @param depth The current depth of the recursion.
 * @return The best score that can be achieved by the AI player.
 */
int TicTacToe::getMaxScore(char board[3][3], int depth) {
    int best = INT_MIN;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                best = std::max(best, minimax(board, depth + 1, false));
                board[i][j] = ' ';
            }
        }
    }
    return (best);
}

/**
 * @brief Determines the best score for the minimizer (human) player.
 * 
 * This function iterates over all possible moves, applies the Minimax algorithm recursively,
 * and returns the lowest score that can be achieved by the human player.
 * 
 * @param board The current state of the game bominird.
 * @param depth The current depth of the recursion.
 * @return The best score that can be achieved by the human player.
 */
int TicTacToe::getMinScore(char board[3][3], int depth) {
    int best = INT_MAX;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'X';
                best = std::min(best, minimax(board, depth + 1, true));
                board[i][j] = ' ';
            }
        }
    }
    return (best);
}

/**mini
 * @brief Finds the best move for the AI player using the Minimax algorithm.
 * 
 * This function evaluates all possible moves for the AI player ('O') on the current board state
 * and returns the move with the highest value determined by the Minimax algorithm.
 * 
 * @return A pair of integers representing the row and column of the best move for the AI player.
 *         If no moves are possible, returns (-1, -1).
 */
std::pair<int, int> TicTacToe::findBestMove() {
    int bestVal = INT_MIN;
    std::pair<int, int> bestMove;
    bestMove.first = -1;
    bestMove.second = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int moveVal = minimax(board, 0, false);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestMove.first = i;
                    bestMove.second = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return (bestMove);
}

// ===================================================================================================
// =========================================== Utils PART ============================================
// ===================================================================================================


// ============ Getters ============

std::string TicTacToe::getBoard() const {
    std::string board_str = "";
    board_str += "    0   1   2\n";
    board_str += "  ╒═══╤═══╤═══╕\n";
    
    for (int i = 0; i < 3; i++) {
        std::stringstream ss;
        ss << i;
        board_str += ss.str() + " │";
        for (int j = 0; j < 3; j++) {
            board_str += " ";
            board_str += board[i][j];
            board_str += " │";
        }
        board_str += "\n";
        if (i < 2)
            board_str += "  ├———┼———┼———┤\n";
    }
    board_str += "  ╘═══╧═══╧═══╛";
    return board_str;
}

/**
 * @brief Converts a string move to a pair of integers representing the row and column.
 *
 * This function takes a string move in the format "row-col", parses it, and converts it
 * to zero-based indices for internal use in the game.
 *
 * @param move The move string in the format "row-col".
 * @return A pair of integers representing the row and column.
 */
std::pair<int, int> TicTacToe::getCorrMove(const std::string& move) const {
    int row, col;
    char dash;
    std::stringstream ss(move);
    
    ss >> row >> dash >> col;
    return std::make_pair(row, col);  // Utilisez std::make_pair pour C++98
}

// ============ Setters ============