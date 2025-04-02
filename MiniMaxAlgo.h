/**
 * @file MiniMaxAlgo.h
 * @brief Implements the Minimax algorithm with alpha-beta pruning for AI gameplay in Checkers.
 * 
 * Contains logic for recursive game state evaluation, move simulation,
 * board evaluation heuristics, and iterative deepening with time constraints.
 * This class is used by the AI player to decide optimal moves.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef MINIMAXALGO_H
#define MINIMAXALGO_H

#include "checkersboard.h"  // Use CheckersBoard instead of Board
#include "piece.h"          // Include Piece class
#include <utility>
#include <limits>
#include <chrono>
#include <tuple> // for std::tuple

/**
 * @class MiniMaxAlgo
 * @brief Implements the Minimax Algorithm with Alpha-Beta Pruning for AI decision-making.
 * 
 * This class evaluates board states and selects optimal moves for the AI.
 */
class MiniMaxAlgo {
private:
    int maxDepth;  ///< Maximum search depth for Minimax

public:
    /**
     * @brief Constructor for MiniMaxAlgo.
     * @param depth The maximum depth for Minimax recursion.
     */
    MiniMaxAlgo(int depth);

    /**
     * @brief Evaluates the board state.
     */
    int evaluateBoard(CheckersBoard& board);

    /**
     * @brief Executes the Minimax algorithm with Alpha-Beta Pruning.
     * @param board The current game board.
     * @param depth The current depth of recursion.
     * @param isMaximizing True if maximizing AI's advantage, false if minimizing.
     * @param alpha The alpha value for pruning.
     * @param beta The beta value for pruning.
     * @return A pair containing the best score and the best move as a tuple: (pieceId, targetRow, targetCol).
     */
    std::pair<int, std::tuple<int, int, int>>
    minimax(CheckersBoard& board, int depth, bool isMaximizing, int alpha = -9999, int beta = 9999);

    /**
     * @brief Gets the best move within a given time limit using iterative deepening.
     * @param board The current game board.
     * @param timeLimitMillis The time limit in milliseconds.
     * @return The best move as a tuple: (pieceId, targetRow, targetCol).
     */
    std::tuple<int, int, int> getBestTimedMove(CheckersBoard& board, int timeLimitMillis);
};

#endif // MINIMAXALGO_H
