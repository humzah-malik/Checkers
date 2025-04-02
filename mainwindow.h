/**
 * @file mainwindow.h
 * @brief Implements the MainWindow class which serves as the central hub for the game application.
 * 
 * Handles layout switching via QStackedWidget, initializes UI pages, manages music and sound effects,
 * connects signals for navigation, and launches the Checkers game logic. It supports both PvP and PvAI modes,
 * and integrates instructions, settings, and game description screens.
 * 
 * @author Humzah Zahid Malik, Lok Hui, Krish Bhavin Patel
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QSoundEffect>
#include <functional>

#include "checkersmenu.h"
#include "checkersmanager.h"
#include "gamepage.h"
#include "frontpage.h"
#include "pvppage.h"
#include "pvaipage.h"
#include "instructionspage.h"
#include "settingsdialog.h"

/**
 * @class MainWindow
 * @brief Central controller for all major screens and game states in the application.
 *
 * Manages screen transitions using QStackedWidget, launches games dynamically,
 * and handles global settings like sound and music.
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window controller.
     * @param parent Optional QWidget parent.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    // Plays the selection click sound.
    void playSelectSound();

    // Displays the game description page (text-based overview).
    void showGameDescriptionPage();

private slots:
    // Starts a new game based on user-selected options.
    void onStartGame(int numPlayers, const QString &difficulty, bool showHints, bool soundEnabled, bool aiEnable);

    // Returns to the initial front page.
    void onBackToFrontPage();

    // Returns to the game selection screen (e.g. Checkers menu).
    void onBackToGameMenu();

    // Quits the application.
    void onExitApp();

    // Shows the classic Checkers menu.
    void showCheckersMenu();

    // Shows 2-player local game setup page.
    void showPvPPage();

    // Shows Player vs AI game setup page.
    void showPvAIPage();

    // Opens the instructions page.
    void showInstructionsPage();

    // Launches the settings dialog.
    void openSettingsDialog();

    // Enables/disables background music.
    void toggleMusic(bool enabled);

    // Enables/disables game sound effects.
    void toggleSoundEffects(bool enabled);

    // Game SFX methods
    void playPieceMoveSound();
    void playPieceCaptureSound();
    void playKingPromotionSound();
    void playTurnChangeSound();
    void playGameEndSound();

private:
    /**
     * @brief Struct defining a registered game type.
     */
    struct GameEntry {
        std::function<QWidget*(QWidget*)> createMenu;      // Factory to create the game setup menu
        std::function<QWidget*()> createBoard;             // Factory to create the game board widget
        std::function<QObject*(QObject*)> createManager;   // Factory to create the game manager/controller
        QString title;                                     // Game title for UI
    };

    QMap<QString, GameEntry> gameRegistry;  // Registry of supported games
    QString currentGameKey;                 // Active game key for dynamic switching

    QStackedWidget *stack;                  // Main layout stack (page navigation)
    FrontPage *frontPage;                   // Landing screen

    QWidget *currentMenu = nullptr;         // Active menu widget
    QWidget *currentGameWidget = nullptr;   // Active board widget
    GamePage *activeGamePage = nullptr;     // Container for board + controls
    QObject *gameManager = nullptr;         // Controller/logic for current game

    QMediaPlayer *backgroundMusicPlayer;    // Music playback
    QAudioOutput *audioOutput;              // Audio output handler
    bool isMusicEnabled = true;             // Tracks music state

    QMap<QString, QSoundEffect*> soundEffects; // Cached sound effects (move, capture, etc.)
    bool isSoundEffectsEnabled = true;         // Tracks sound effects state

    // Registers all playable games.
    void registerGames();

    // Loads and prepares sound effects for gameplay.
    void initializeSoundEffects();
};

#endif // MAINWINDOW_H