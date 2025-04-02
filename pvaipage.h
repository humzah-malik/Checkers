/**
 * @file pvaipage.h
 * @brief Implements the UI for the Player vs AI configuration page in the Checkers game.
 * 
 * Allows the user to select difficulty and start a PvAI match. Also includes
 * navigation controls such as back, settings, and exit buttons.
 * 
 * Handles layout setup, difficulty adjustment, and signal connections to trigger
 * the game start with selected options.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef PVAIPAGE_H
#define PVAIPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @class PvAIPage
 * @brief UI page for selecting Player vs AI game settings.
 * 
 * Allows user to pick difficulty, toggle options, and start the game.
 */
class PvAIPage : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the PvAI settings page.
     * @param parent The parent widget.
     */
    explicit PvAIPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when user clicks Back button to return to menu.
     */
    void backToMenu();

    /**
     * @brief Emitted when user starts the game with selected settings.
     * @param difficulty Difficulty level as a string (e.g., "Easy").
     * @param showHints True if hints should be shown.
     * @param soundEnabled True if sound is enabled.
     */
    void startPvAIGame(const QString &difficulty, bool showHints, bool soundEnabled);

    /**
     * @brief Emitted when the user opens the settings dialog.
     */
    void settingsClicked();

private slots:
    /**
     * @brief Handles when the Start button is clicked.
     */
    void onStartClicked();

    /**
     * @brief Lowers the difficulty level when "-" is clicked.
     */
    void onDecreaseDifficulty();

    /**
     * @brief Raises the difficulty level when "+" is clicked.
     */
    void onIncreaseDifficulty();

private:
    QLabel *titleLabel;           // Displays the page title
    QLabel *difficultyLabel;      // Shows current difficulty level

    QPushButton *decreaseButton;  // "-" button to decrease difficulty
    QPushButton *increaseButton;  // "+" button to increase difficulty
    QPushButton *startButton;     // Starts the game
    QPushButton *backButton;      // Returns to main menu
    QPushButton *exitButton;      // Optional exit button (unused?)
    QPushButton *settingsButton;  // Opens settings dialog

    QStringList difficulties;     // List of difficulty levels
    int difficultyIndex;          // Current index in difficulty list
};

#endif // PVAIPAGE_H