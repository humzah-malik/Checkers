/**
 * @file AI.cpp
 * @brief Implements the AI class responsible for making moves in the Checkers game.
 * 
 * This class handles AI behavior, including selecting a move using either random selection
 * or the Minimax algorithm, depending on the difficulty level selected by the user.
 * 
 * @author Humzah Zahid Malik
 */

#include "AI.h"
#include <cstdlib>
#include <ctime>
#include <QList>
#include <QVector>
#include <QDebug>
#include <tuple> // for std::tuple and std::get

/**
 * @brief Constructs the AI player using a vector of pieces.
 * 
 * Initializes the base Player class with a name and list of pieces. Also seeds the random generator
 * and initializes the minimax algorithm based on the difficulty.
 * 
 * @param aiDifficulty The difficulty level of the AI (1 = easy, 2 = medium, 3 = hard).
 * @param aiPieces The vector of Piece pointers that belong to the AI.
 */
AI::AI(int aiDifficulty, std::vector<Piece*> aiPieces) 
    : Player("AI", QList<Piece*>(aiPieces.begin(), aiPieces.end())),
      difficulty(aiDifficulty),
      minimaxAlgo(aiDifficulty * 2)
{
    std::srand(std::time(nullptr));
}

/**
 * @brief Alternate constructor using QList instead of vector.
 * 
 * Converts the QList to a std::vector and calls the main constructor.
 * 
 * @param aiDifficulty The difficulty level of the AI.
 * @param aiPieces The list of Piece pointers that belong to the AI.
 */
AI::AI(int aiDifficulty, QList<Piece*> aiPieces) 
    : AI(aiDifficulty, std::vector<Piece*>(aiPieces.begin(), aiPieces.end())) {}

/**
 * @brief Determines the best move for the AI based on its difficulty level.
 * 
 * - Easy: makes a random move.
 * - Medium: uses the minimax algorithm with 200 ms time-limit.
 * - Hard: uses the minimax algorithm with 350 ms time-limit.
 * 
 * @param board The current state of the checkers board.
 * @return A pair; the Piece to move and its target position (row, col).
 */
std::pair<Piece*, std::pair<int, int>> AI::getBestMove(CheckersBoard& board) {
    
    // Difficulty 1 = Easy mode: use random move
    if (difficulty == 1) {
        return getRandomMove(board);  // Calls helper function to pick a random valid move
    } 
    // Difficulty 2 = Medium mode: use minimax with 200ms time limit
    else if (difficulty == 2) {
        std::tuple<int, int, int> move = minimaxAlgo.getBestTimedMove(board, 200);  // Run minimax for 200ms
        
        int pieceId = std::get<0>(move);       // Extract ID of selected piece
        int targetRow = std::get<1>(move);     // Extract target row
        int targetCol = std::get<2>(move);     // Extract target column
        
        Piece* bestPiece = board.getPieceById(pieceId);  // Get the actual piece from its ID
        return { bestPiece, { targetRow, targetCol } };  // Return the move as a pair
    } 
    // Difficulty 3 or higher = Hard mode: use minimax with 350ms
    else {
        std::tuple<int, int, int> move = minimaxAlgo.getBestTimedMove(board, 350);

        int pieceId = std::get<0>(move);       // Extract ID of selected piece
        int targetRow = std::get<1>(move);     // Extract target row
        int targetCol = std::get<2>(move);     // Extract target column
        
        Piece* bestPiece = board.getPieceById(pieceId);  // Get the actual piece using its ID
        return { bestPiece, { targetRow, targetCol } };  // Return the selected move
    }
}

/**
 * @brief Makes a random valid move for the AI.
 * 
 * Iterates over all AI-controlled pieces and collects valid moves, then randomly selects one.
 * If no valid move is found, returns a null pointer and invalid coordinates.
 * 
 * @param board The current state of the checkers board.
 * @return A pair; the randomly selected Piece and its move position (row, col).
 */
std::pair<Piece*, std::pair<int, int>> AI::getRandomMove(CheckersBoard& board) {
    std::vector<std::pair<Piece*, std::pair<int, int>>> possibleMoves;
    
    // Get the updated list of AI pieces from the board
    QList<Piece*> currentPieces = board.getAIPieces();

    for (Piece* piece : currentPieces) {
        if (!piece) continue;

        // Get fresh pointer to the piece by ID
        Piece* currentPiece = board.getPieceById(piece->getId());
        if (!currentPiece) continue;

        // Check every position on the board for valid moves
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (board.isValidMove(currentPiece, row, col)) {
                    possibleMoves.push_back({currentPiece, {row, col}});
                }
            }
        }
    }

    if (!possibleMoves.empty()) {
        return possibleMoves[rand() % possibleMoves.size()];
    }

    // No valid move found
    return {nullptr, {-1, -1}};
}