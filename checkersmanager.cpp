/**
 * @file checkersmanager.cpp
 * @brief Implements the CheckersManager class for managing the overall game state and control flow.
 * 
 * This file manages game startup, AI and player interaction, turn logic, 
 * undo functionality, and win condition checks. It connects the game board to 
 * either player-vs-player or player-vs-AI logic depending on selected settings.
 * 
 * @author Lok Hui
 */

#include "checkersmanager.h"
#include <QTimer>

/**
 * @brief Constructor for CheckersManager.
 * 
 * Initializes the gameOver flag.
 */
CheckersManager::CheckersManager(QObject *parent) : QObject(parent)
{
    gameOver = false;
}

/**
 * @brief Starts a new game with provided settings.
 * 
 * Connects board signals and initializes players or AI based on mode.
 */
void CheckersManager::onStart(int numPlayers, const QString &difficulty, bool showHints, bool soundEffectEnabled, bool aiEnabled)
{
    qDebug() << "Controller: Start game signaled with parameters:"
             << "numPlayers=" << numPlayers
             << "difficulty=" << difficulty
             << "showHints=" << showHints
             << "soundEnabled=" << soundEffectEnabled
             << "AI Enabled" << aiEnabled;

    // Connect board signals to manager slots
    connect(board, &CheckersBoard::moveCompleted, this, &CheckersManager::makeAIMove);
    connect(board, &CheckersBoard::gameCheck, this, &CheckersManager::checkWinCondition);

    // Initialize player 1 as human (Black)
    player1 = new Player("player 1", board->getPieces(PieceColor::Black));

    if (numPlayers == 1) {
        // PvAI mode
        int difficultyLevel = (difficulty == "Easy") ? 1 : (difficulty == "Medium") ? 2 : 3;
        aiPlayer = new AI(difficultyLevel, board->getPieces(PieceColor::Red));
        player2 = aiPlayer;
    } else {
        // PvP mode
        player2 = new Player("Player 2", board->getPieces(PieceColor::Red));
    }
}

/**
 * @brief Makes the AI play a move if it's its turn.
 * 
 * Executes AI move using a delayed QTimer.
 */
void CheckersManager::makeAIMove()
{
    if (gameOver) {
        return;
    }

    // Only proceed if AI exists and it's its turn
    if (aiPlayer && board->getCurrentTurn() == PieceColor::Red) {

        // Delay the move slightly for UI responsiveness
        QTimer::singleShot(50, this, [=]() {
            auto bestMove = aiPlayer->getBestMove(*board);
            if (bestMove.first) {
                board->handleMove(bestMove.first, bestMove.second.first, bestMove.second.second);
            } else {
            }
        });
    }
}

/**
 * @brief Checks the win condition for both sides.
 * 
 * Considers valid moves and piece counts to detect win/stalemate/draw.
 */
void CheckersManager::checkWinCondition()
{
    if (!board) return;

    bool redHasMoves = board->hasValidMoves(PieceColor::Red);
    bool blackHasMoves = board->hasValidMoves(PieceColor::Black);

    int redCount = board->getPieces(PieceColor::Red).size();
    int blackCount = board->getPieces(PieceColor::Black).size();

    // Stalemate: no valid moves for both sides
    if (!redHasMoves && !blackHasMoves) {
        if (redCount > blackCount) {
        } else if (blackCount > redCount) {
        } else {
        }
        gameOver = true;
        board->emit gameEnded();
        return;
    }

    if (!redHasMoves) {
    gameOver = true;
    board->emit gameEnded();
    return;
}

if (!blackHasMoves) {
    gameOver = true;
    board->emit gameEnded();
    return;
}
}

/**
 * @brief Assigns the given board instance to the manager.
 * 
 * @param board Pointer to a CheckersBoard instance.
 */
void CheckersManager::setBoard(CheckersBoard *board)
{
    this->board = board;
}

/**
 * @brief Undoes the last move or pair of moves, depending on mode.
 * 
 * In PvAI: undoes both AI and player move. In PvP: undo last two player moves.
 */
void CheckersManager::undoMove()
{
    if (!board || gameOver) return;

    if (aiPlayer) {
        // PvAI Mode — only the user (Black) can undo
        if (userUndosLeft <= 0) {
            return;
        }

        // Undo AI move + user move
        board->undoLastMove();  // Undo AI's move
        board->undoLastMove();  // Undo user's move
        board->forceTurn(PieceColor::Black); // Force back to user

        userUndosLeft--;
        emit userUndoCountUpdated(userUndosLeft); // Update UI
        return;
    }

    // PvP mode — alternate undo pools for Red and Black
    PieceColor current = board->getCurrentTurn();

    if (current == PieceColor::Red) {
        if (redUndosLeft <= 0) {
            return;
        }

        board->undoLastMove();  // Undo Black's last move
        board->undoLastMove();  // Undo Red's previous move
        board->forceTurn(PieceColor::Red);

        redUndosLeft--;
        emit undoCountsUpdated(redUndosLeft, blackUndosLeft);
    } else {
        if (blackUndosLeft <= 0) {
            return;
        }

        board->undoLastMove();  // Undo Red's last move
        board->undoLastMove();  // Undo Black's previous move
        board->forceTurn(PieceColor::Black);

        blackUndosLeft--;
        emit undoCountsUpdated(redUndosLeft, blackUndosLeft);
    }
}