/**
 * @file checkersmenu.h
 * @brief Implements the main menu UI for the Checkers game.
 * 
 * Handles game mode selection (PvP, PvAI), difficulty adjustment,
 * and links to instructions, settings, and game description pages.
 * 
 * @author Lok Hui
 */

#ifndef CHECKERSMENU_H
#define CHECKERSMENU_H

#include <QDebug>
#include <QCheckBox>
#include <QRadioButton>  // Used for game mode selection
#include "npcontainerwidget.h"

/**
 * @class CheckersMenu
 * @brief UI widget for setting up game preferences before starting a Checkers match.
 * 
 * Allows users to configure game mode, difficulty, and other settings.
 */
class CheckersMenu : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the menu widget.
     * @param parent Optional parent QWidget.
     */
    explicit CheckersMenu(QWidget *parent = nullptr);

private:
    QLabel *titleLabel;                        // Main title label
    NpContainerWidget *nPlayersContainer;      // Widget for selecting number of players

    QLabel *optionsLabel;                      // Label for the "Options" section

    QLabel *difficultyTitleLabel, *difficultyLabel; // Labels for difficulty setting
    ArrowButton *addDifficulty, *minusDifficulty;   // Buttons to increase/decrease difficulty

    QCheckBox *showHintsCheckBox;              // Toggle to enable hint highlighting
    QCheckBox *soundEffectCheckBox;            // Toggle to enable sound effects

    QPushButton *startButton;                  // Starts the game
    QPushButton *backButton;                   // Navigates back to main/front page
    QPushButton *exitButton;                   // Exits the application
    QPushButton *settingsButton;               // Opens the settings panel (if implemented)

    int difficultyIndex;                       // Current index in difficulty list
    QStringList difficulties;                  // List of difficulty levels

    QRadioButton *pvpMode;                     // Game mode: Player vs Player
    QRadioButton *pvaiMode;                    // Game mode: Player vs AI
    bool isAIEnabled = false;                  // Flag indicating whether AI mode is selected

    /**
     * @brief Builds and lays out the menu UI.
     */
    void setupUI();

    /**
     * @brief Updates the displayed difficulty label based on difficultyIndex.
     */
    void updateDifficultyLabel();

signals:
    /**
     * @brief Emitted when the Start button is clicked.
     * 
     * @param numPlayers Number of players (1 = PvAI, 2 = PvP)
     * @param difficulty Difficulty level as string
     * @param showHintsIsEnabled Whether hint highlighting is enabled
     * @param soundEffectIsEnabled Whether sound effects are enabled
     * @param aiEnabled Whether the game should use AI
     */
    void startGameSignaled(
        int numPlayers,
        const QString &difficulty,
        bool showHintsIsEnabled,
        bool soundEffectIsEnabled,
        bool aiEnabled
    );

    /**
     * @brief Emitted when the Back button is clicked.
     */
    void backToFrontPage();

    /**
     * @brief Emitted when the Exit button is clicked.
     */
    void exitAppRequested();

    /**
     * @brief Emitted when PvP mode is selected.
     */
    void pvpSelected();

    /**
     * @brief Emitted when PvAI mode is selected.
     */
    void pvaiSelected();

    /**
     * @brief Emitted when "Instructions" are selected (if implemented).
     */
    void instructionsSelected();

    /**
     * @brief Emitted when Settings is requested (optional).
     */
    void settingsRequested();

private slots:
    /**
     * @brief Triggered when the Start button is clicked.
     */
    void onStartClicked();

    /**
     * @brief Triggered when difficulty is decreased.
     */
    void minusDifficultySlot();

    /**
     * @brief Triggered when difficulty is increased.
     */
    void addDifficultySlot();

    /**
     * @brief Updates the selected game mode based on radio button change.
     */
    void updateGameMode();
};

#endif // CHECKERSMENU_H