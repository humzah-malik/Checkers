/**
 * @file checkersmanager..h
 * @brief Implements the CheckersManager class for managing the overall game state and control flow.
 * 
 * This file manages game startup, AI and player interaction, turn logic, 
 * undo functionality, and win condition checks. It connects the game board to 
 * either player-vs-player or player-vs-AI logic depending on selected settings.
 * 
 * @author Lok Hui
 */

#ifndef CHECKERSMANAGER_H
#define CHECKERSMANAGER_H

#include <QObject>
#include "checkersboard.h"
#include "Player.h"
#include "AI.h"

/**
 * @class CheckersManager
 * @brief Manages game state, player turns, AI, and undo logic.
 * 
 * Acts as the controller between the board, players, and UI triggers.
 */
class CheckersManager : public QObject {
    Q_OBJECT

private:
    CheckersBoard *board;    // Pointer to the main game board
    Player *player1;         // First player (usually human)
    Player *player2;         // Second player (can be AI or human)
    AI *aiPlayer;            // AI logic handler (used in PvAI mode)

    bool gameOver = false;   // Tracks whether the game has ended

    // For PvAI mode: assume human is always Black
    int userUndosLeft = 2;   // Undo count for the user in PvAI mode

public:
    /**
     * @brief Constructor for the game manager.
     * @param parent Optional Qt parent object.
     */
    explicit CheckersManager(QObject *parent = nullptr);

    /**
     * @brief Assigns the board instance to the manager.
     * @param board Pointer to an existing CheckersBoard.
     */
    void setBoard(CheckersBoard *board);

    /**
     * @brief Remaining undos for Red player (PvP).
     */
    int redUndosLeft = 2;

    /**
     * @brief Remaining undos for Black player (PvP).
     */
    int blackUndosLeft = 2;

signals:
    /**
     * @brief Emitted when either player's undo counts change.
     * @param redLeft Remaining undos for Red.
     * @param blackLeft Remaining undos for Black.
     */
    void undoCountsUpdated(int redLeft, int blackLeft);

    /**
     * @brief Emitted when user's undo count changes (PvAI mode).
     * @param remaining Undos left for the user.
     */
    void userUndoCountUpdated(int remaining);

public slots:
    /**
     * @brief Starts a new game with the given configuration.
     * @param numPlayers 1 = PvAI, 2 = PvP.
     * @param difficulty AI difficulty level ("Easy", "Medium", "Hard").
     * @param showHints Whether to enable move hints.
     * @param soundEffectEnabled Whether sound is enabled.
     * @param aiEnabled Whether AI is enabled.
     */
    void onStart(
        int numPlayers,
        const QString &difficulty,
        bool showHints,
        bool soundEffectEnabled,
        bool aiEnabled
    );

    /**
     * @brief Triggers the AI to make a move (PvAI mode).
     */
    void makeAIMove();

    /**
     * @brief Checks if the current player has won the game.
     */
    void checkWinCondition();

    /**
     * @brief Undoes the last move, if allowed.
     */
    void undoMove();
};

#endif // CHECKERSMANAGER_H