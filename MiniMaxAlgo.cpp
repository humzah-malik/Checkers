/**
 * @file MiniMaxAlgo.cpp
 * @brief Implements the Minimax algorithm with alpha-beta pruning for AI gameplay in Checkers.
 * 
 * Contains logic for recursive game state evaluation, move simulation,
 * board evaluation heuristics, and iterative deepening with time constraints.
 * This class is used by the AI player to decide optimal moves.
 * 
 * @author Humzah Zahid Malik
 */

#include "MiniMaxAlgo.h"
#include <QDebug>
#include <algorithm> // for std::max and std::min

/// @brief Constructor that sets max search depth.
/// @param depth Max depth for minimax search.
MiniMaxAlgo::MiniMaxAlgo(int depth) : maxDepth(depth) {}

/// @brief Minimax algorithm with alpha-beta pruning.
/// @param board Current game board.
/// @param depth Recursion depth.
/// @param isMaximizing True if AI's turn.
/// @param alpha Alpha value for pruning.
/// @param beta Beta value for pruning.
/// @return Pair of best score and best move (pieceId, row, col).
std::pair<int, std::tuple<int, int, int>>
MiniMaxAlgo::minimax(CheckersBoard& board, int depth, bool isMaximizing, int alpha, int beta) {
    // Stop if depth is 0 or game is over
    if (depth == 0 || board.checkwin(PieceColor::Red) || board.checkwin(PieceColor::Black))
        return { evaluateBoard(board), std::make_tuple(-1, -1, -1) };

    // Init best move and score
    std::tuple<int, int, int> bestMove = std::make_tuple(-1, -1, -1);
    int bestScore = isMaximizing ? std::numeric_limits<int>::min()
                                 : std::numeric_limits<int>::max();

    // Choose pieces based on current player
    const QList<Piece*> pieces = isMaximizing ? board.getAIPieces() : board.getOpponentPieces();

    // Loop through each piece
    for (Piece* piece : pieces) {
        if (!piece) continue;

        // Try every square
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (!board.isValidMove(piece, row, col)) continue;

                // Clone board for simulation
                CheckersBoard* tempBoard = board.clone();

                // Get matching piece in clone
                Piece* tempPiece = tempBoard->getPieceAt(piece->getRow(), piece->getCol());
                if (!tempPiece) {
                    delete tempBoard;
                    continue;
                }

                // Simulate the move
                tempBoard->handleMove(tempPiece, row, col);

                // Chain captures if possible
                int maxChain = 12; // avoid infinite loops
                while (tempBoard->isCaptureAvailable(tempPiece) && maxChain-- > 0) {
                    bool foundNext = false;
                    for (int r = 0; r < 8 && !foundNext; ++r) {
                        for (int c = 0; c < 8; ++c) {
                            if (tempBoard->isValidMove(tempPiece, r, c)) {
                                tempBoard->handleMove(tempPiece, r, c);
                                foundNext = true;
                                break;
                            }
                        }
                    }
                    if (!foundNext) break;
                }

                // Recursively evaluate
                int score = minimax(*tempBoard, depth - 1, !isMaximizing, alpha, beta).first;
                delete tempBoard;

                // Update best score and move
                if (isMaximizing && score > bestScore) {
                    bestScore = score;
                    bestMove = std::make_tuple(piece->getId(), row, col);
                    alpha = std::max(alpha, bestScore);
                } else if (!isMaximizing && score < bestScore) {
                    bestScore = score;
                    bestMove = std::make_tuple(piece->getId(), row, col);
                    beta = std::min(beta, bestScore);
                }

                // Prune if possible
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }
    }

    return { bestScore, bestMove };
}

/// @brief Evaluates current board state.
/// @param board Board to evaluate.
/// @return Score for AI (Red positive, Black negative).
int MiniMaxAlgo::evaluateBoard(CheckersBoard& board) {
    int score = 0;

    // Go through all items on the board
    for (QGraphicsItem* item : board.scene()->items()) {
        Piece* piece = dynamic_cast<Piece*>(item);
        if (!piece) continue;

        // Base value: higher for king
        int base = piece->isKing() ? 4 : 3;

        // Bonus for advancing forward
        int adv = (piece->getColor() == PieceColor::Red)
                  ? static_cast<int>(piece->getRow() * 0.5)
                  : static_cast<int>((7 - piece->getRow()) * 0.5);

        // Bonus for being near center
        int center = (piece->getRow() >= 2 && piece->getRow() <= 5 &&
                      piece->getCol() >= 2 && piece->getCol() <= 5) ? 1 : 0;

        // Bonus if captures are available
        int capture = board.isCaptureAvailable(piece) ? 3 : 0;

        // Bonus for move options
        int mobility = 0;
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (board.isValidMove(piece, r, c))
                    mobility++;
            }
        }
        int mobilityBonus = mobility / 2;

        // Final piece score
        int pieceScore = base + adv + center + capture + mobilityBonus;

        // Add/subtract to total score
        score += (piece->getColor() == PieceColor::Red) ? pieceScore : -pieceScore;
    }

    return score;
}

/// @brief Iterative deepening Minimax with time limit.
/// @param board Game board.
/// @param timeLimitMillis Time cap in milliseconds.
/// @return Best move found (pieceId, row, col).
std::tuple<int, int, int> MiniMaxAlgo::getBestTimedMove(CheckersBoard& board, int timeLimitMillis) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now(); // start timer

    std::tuple<int, int, int> bestMove = std::make_tuple(-1, -1, -1);

    // Increase depth gradually
    for (int d = 1; d <= maxDepth; ++d) {
        // Check time
        auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        if (elapsed >= timeLimitMillis) break;

        // Clone board and run minimax
        CheckersBoard* tempBoard = board.clone();
        auto result = minimax(*tempBoard, d, true, -9999, 9999);
        delete tempBoard;

        bestMove = std::get<1>(result);

        // Check time again after iteration
        elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        if (elapsed >= timeLimitMillis) break;
    }

    auto total = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

    return bestMove;
}