/**
 * @file gamepage.h
 * @brief Implements the main in-game UI for a Checkers match.
 * 
 * Sets up the game layout, controls (undo, back, settings, exit), and 
 * displays turn indicators, undo counts, and game-over dialogs.
 * Handles user interactions and connects to CheckersBoard events.
 * 
 * @author Lok Hui
 */

#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "arrowbutton.h"
#include "piece.h"         // For PieceColor enum
#include "checkersboard.h" // For game logic

/**
 * @class GamePage
 * @brief Main UI container for a Checkers game session.
 *
 * Displays the game board, turn indicator, undo counts, and control buttons.
 */
class GamePage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the GamePage layout.
     * @param gameWidget The game board widget.
     * @param gameTitle The title of the current game mode.
     * @param parent Optional parent widget.
     */
    explicit GamePage(QWidget *gameWidget, const QString &gameTitle, QWidget *parent = nullptr);

signals:
    // Emitted when the user clicks the "Back" button.
    void backToMenu();

    // Emitted when the user clicks the "Exit" button.
    void exitApp();

    // Emitted when the user clicks the "Undo" button.
    void requestUndo();

    // Emitted when the user clicks the "Settings" button.
    void settingsClicked();

    // Emitted when the user chooses to replay the game after game over.
    void playAgainRequested();

public slots:
    // Handles undo button logic and emits requestUndo().
    void onUndoClicked();

    // Updates both undo labels (for PvP mode).
    void updateUndoLabels(int redLeft, int blackLeft);

    // Updates user undo label (for PvAI mode).
    void updateUserUndoLabel(int remaining);

    // Updates the turn indicator text and color.
    void updateTurnIndicator(PieceColor newTurn);

    // Displays the game-over dialog with winner message and options.
    void showGameOverDialog(PieceColor winner);

private:
    QPushButton *backButton, *exitButton, *undoButton; // Control buttons
    QLabel *redUndosLabel;                             // Red player's undo label
    QLabel *blackUndosLabel;                           // Black player's undo label
    QLabel *turnIndicatorLabel;                        // Displays current player's turn
};

#endif