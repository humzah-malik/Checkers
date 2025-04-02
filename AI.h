/**
 * @file AI.h
 * @brief Implements the AI class responsible for making moves in the Checkers game.
 * 
 * This class handles AI behavior, including selecting a move using either random selection
 * or the Minimax algorithm, depending on the difficulty level selected by the user.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef AI_H
#define AI_H

#include "Player.h"
#include "MiniMaxAlgo.h"

/**
 * @class AI
 * @brief Represents an AI-controlled player in the Checkers game.
 * 
 * The AI class extends Player and uses either random move selection
 * or the Minimax algorithm to decide its next move, depending on
 * the selected difficulty by the user.
 */
class AI : public Player {
private:
    int difficulty;           ///< The difficulty level of the AI (1 = Easy, 2 = Medium, 3+ = Hard)
    MiniMaxAlgo minimaxAlgo;  ///< Instance of the Minimax algorithm used for decision-making

public:
    /**
     * @brief Constructs the AI player using a std::vector of Piece pointers.
     * 
     * @param aiDifficulty The difficulty level of the AI.
     * @param aiPieces A vector of Piece pointers belonging to the AI.
     */
    AI(int aiDifficulty, std::vector<Piece*> aiPieces);

    /**
     * @brief Constructs the AI player using a QList of Piece pointers.
     * 
     * Converts the QList to std::vector and forwards to the main constructor.
     * 
     * @param aiDifficulty The difficulty level of the AI.
     * @param aiPieces A QList of Piece pointers belonging to the AI.
     */
    AI(int aiDifficulty, QList<Piece*> aiPieces);

    /**
     * @brief Gets the best move for the AI to play based on the current board state.
     * 
     * Uses random selection (easy) or minimax (medium/hard) depending on difficulty.
     * 
     * @param board The current state of the checkers board.
     * @return A pair consisting of the Piece to move and its target position (row, col).
     */
    std::pair<Piece*, std::pair<int, int>> getBestMove(CheckersBoard& board);

    /**
     * @brief Gets a random valid move for the AI to play.
     * 
     * Iterates over all of the AI’s pieces and returns a random valid move.
     * 
     * @param board The current state of the checkers board.
     * @return A pair consisting of the Piece to move and its target position (row, col),
     *         or {nullptr, {-1, -1}} if no valid move is found.
     */
    std::pair<Piece*, std::pair<int, int>> getRandomMove(CheckersBoard& board);
};

#endif