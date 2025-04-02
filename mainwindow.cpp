/**
 * @file mainwindow.cpp
 * @brief Implements the MainWindow class which serves as the central hub for the game application.
 * 
 * Handles layout switching via QStackedWidget, initializes UI pages, manages music and sound effects,
 * connects signals for navigation, and launches the Checkers game logic. It supports both PvP and PvAI modes,
 * and integrates instructions, settings, and game description screens.
 * 
 * @author Humzah Zahid Malik, Lok Hui, Krish Bhavin Patel
 */

#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "checkersmenu.h"
#include "checkersmanager.h"
#include "settingsdialog.h"
#include "gamedescriptionpage.h"


/**
 * @brief Constructs the main window, initializes UI, front page, music, and sound effects.
 * 
 * This is the central controller for the application. It sets up the main stack,
 * connects the front page actions, initializes audio, and registers game types.
 */
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    // Set background color for the entire app
    this->setStyleSheet("background-color: #003153;");
    
    // Create stacked layout system to manage screen switching
    stack = new QStackedWidget(this);
    
    // Initialize and connect the front page
    frontPage = new FrontPage(this);
    connect(frontPage, &FrontPage::checkersClicked, this, [this]() {
        playSelectSound();        // Play click sound
        showCheckersMenu();       // Navigate to Checkers menu
    });

    connect(frontPage, &FrontPage::exitAppRequested, this, [this]() {
        playSelectSound();        // Play click sound
        onExitApp();              // Exit the app
    });

    // Register supported games (e.g., Checkers)
    registerGames();

    // Add front page to the stack and show it
    stack->addWidget(frontPage);
    setLayout(new QVBoxLayout());
    layout()->addWidget(stack);

    setWindowTitle("GameHub");
    resize(1200, 800);

    // Initialize music
    backgroundMusicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    backgroundMusicPlayer->setAudioOutput(audioOutput);
    backgroundMusicPlayer->setSource(QUrl("qrc:/sounds/background_music.mp3"));

    // Handle music playback errors
    connect(backgroundMusicPlayer, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error error, const QString &errorString) {
        QMessageBox::warning(this, "Media Error", "Failed to play background music: " + errorString);
    });

    audioOutput->setVolume(0.25);
    backgroundMusicPlayer->setLoops(QMediaPlayer::Infinite);
    backgroundMusicPlayer->play();
    isMusicEnabled = true;

    // Load all sound effects (move, capture, etc.)
    initializeSoundEffects();
}

/**
 * @brief Loads all required sound effects from the Qt resource system.
 */
void MainWindow::initializeSoundEffects() {
    QStringList soundEffectNames = {
        "piece_move", "piece_capture", "king_promotion", "turn_change", "game_end", "select"
    };

    for (const QString &effectName : soundEffectNames) {
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl(QString("qrc:/sounds/%1.wav").arg(effectName)));
        effect->setVolume(0.5);
        soundEffects[effectName] = effect;

        connect(effect, &QSoundEffect::statusChanged, this, [this, effect, effectName]() {
            if (effect->status() == QSoundEffect::Error) {
                qDebug() << "Sound Effect Error (" << effectName << "): Failed to load sound";
            }
        });
    }
}

/// @brief Plays move sound if enabled.
void MainWindow::playPieceMoveSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("piece_move"))
        soundEffects["piece_move"]->play();
}

/// @brief Plays capture sound if enabled.
void MainWindow::playPieceCaptureSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("piece_capture"))
        soundEffects["piece_capture"]->play();
}

/// @brief Plays king promotion sound if enabled.
void MainWindow::playKingPromotionSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("king_promotion"))
        soundEffects["king_promotion"]->play();
}

/// @brief Plays turn change sound if enabled.
void MainWindow::playTurnChangeSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("turn_change"))
        soundEffects["turn_change"]->play();
}

/// @brief Plays game over sound if enabled.
void MainWindow::playGameEndSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("game_end"))
        soundEffects["game_end"]->play();
}

/// @brief Plays UI selection sound (e.g., button clicks).
void MainWindow::playSelectSound() {
    if (isSoundEffectsEnabled && soundEffects.contains("select"))
        soundEffects["select"]->play();
}

/**
 * @brief Registers available games into the internal game registry.
 */
void MainWindow::registerGames() {
    gameRegistry["Checkers"] = {
        [] (QWidget *parent) { return new CheckersMenu(parent); },
        [] () { return new CheckersBoard(); },
        [] (QObject *parent) { return new CheckersManager(parent); },
        "Checkers"
    };
}

/**
 * @brief Displays the main menu for the currently selected game.
 */
void MainWindow::showCheckersMenu() {
    currentGameKey = "Checkers";
    currentMenu = gameRegistry[currentGameKey].createMenu(this);
    gameManager = gameRegistry[currentGameKey].createManager(this);

    auto *menu = qobject_cast<CheckersMenu *>(currentMenu);
    connect(menu, &CheckersMenu::startGameSignaled, this, &MainWindow::onStartGame);
    connect(menu, &CheckersMenu::backToFrontPage, this, &MainWindow::onBackToFrontPage);
    connect(menu, &CheckersMenu::exitAppRequested, this, &MainWindow::onExitApp);
    connect(menu, &CheckersMenu::settingsRequested, this, &MainWindow::openSettingsDialog);
    connect(menu, &CheckersMenu::pvpSelected, this, &MainWindow::showPvPPage);
    connect(menu, &CheckersMenu::pvaiSelected, this, &MainWindow::showPvAIPage);
    connect(menu, &CheckersMenu::instructionsSelected, this, &MainWindow::showInstructionsPage);

    stack->addWidget(currentMenu);
    stack->setCurrentWidget(menu);
}

/**
 * @brief Starts a new game with the provided configuration.
 */
void MainWindow::onStartGame(int numPlayers, const QString &difficulty, bool showHints, bool soundEnabled, bool aiEnabled)
{
    currentGameWidget = gameRegistry[currentGameKey].createBoard();
    gameManager = gameRegistry[currentGameKey].createManager(this);

    activeGamePage = new GamePage(currentGameWidget, gameRegistry[currentGameKey].title);
    connect(activeGamePage, &GamePage::backToMenu, this, &MainWindow::onBackToGameMenu);
    connect(activeGamePage, &GamePage::exitApp, this, &MainWindow::onExitApp);
    connect(activeGamePage, &GamePage::settingsClicked, this, &MainWindow::openSettingsDialog);
    connect(activeGamePage, &GamePage::playAgainRequested, this, [=]() {
        onStartGame(numPlayers, difficulty, showHints, soundEnabled, aiEnabled);
    });

    stack->addWidget(activeGamePage);
    stack->setCurrentWidget(activeGamePage);

    // Only if current game is Checkers
    if (currentGameKey == "Checkers") {
        CheckersBoard* board = static_cast<CheckersBoard *>(currentGameWidget);
        auto *manager = qobject_cast<CheckersManager *>(gameManager);

        // Connect core gameplay signals
        connect(activeGamePage, &GamePage::requestUndo, manager, &CheckersManager::undoMove);
        connect(manager, &CheckersManager::undoCountsUpdated, activeGamePage, &GamePage::updateUndoLabels);
        connect(manager, &CheckersManager::userUndoCountUpdated, activeGamePage, &GamePage::updateUserUndoLabel);
        connect(board, &CheckersBoard::moveCompleted, this, &MainWindow::playPieceMoveSound);
        connect(board, &CheckersBoard::pieceCaptured, this, &MainWindow::playPieceCaptureSound);
        connect(board, &CheckersBoard::piecePromoted, this, &MainWindow::playKingPromotionSound);
        connect(board, &CheckersBoard::turnChanged, this, &MainWindow::playTurnChangeSound);
        connect(board, &CheckersBoard::gameEnded, this, &MainWindow::playGameEndSound);

        manager->setBoard(board);
        manager->onStart(numPlayers, difficulty, showHints, soundEnabled, aiEnabled);

        // Immediately update the UI depending on the mode
        if (numPlayers == 1) {
            // PvAI: show only user counter
            activeGamePage->updateUserUndoLabel(2);
        } else {
            // PvP: show separate counters
            activeGamePage->updateUndoLabels(2, 2);
        }
    }
}


/// @brief Switches back to the front landing page.
void MainWindow::onBackToFrontPage() {
    stack->setCurrentWidget(frontPage);
}

/// @brief Returns to the current game’s menu screen.
void MainWindow::onBackToGameMenu() {
    stack->setCurrentWidget(currentMenu);
}

/// @brief Quits the application cleanly.
void MainWindow::onExitApp() {
    qApp->quit();
}

/// @brief Shows the Player vs Player setup screen.
void MainWindow::showPvPPage() {
    PvPPage *pvpPage = new PvPPage(this);  // Create PvP config page

    // Connect back button to return to menu
    connect(pvpPage, &PvPPage::backToMenu, this, [this]() {
        playSelectSound();
        stack->setCurrentWidget(currentMenu);
    });

    // Connect start button with selected mode (2 or 4 players)
    connect(pvpPage, &PvPPage::startPvPGame, this, [this](int mode) {
        playSelectSound();
        onStartGame((mode == 2 ? 2 : 4), "Easy", false, false, false);
    });

    // Connect settings icon
    connect(pvpPage, &PvPPage::settingsClicked, this, &MainWindow::openSettingsDialog);

    // Show page
    stack->addWidget(pvpPage);
    stack->setCurrentWidget(pvpPage);
}

/// @brief Shows the Player vs AI setup screen.
void MainWindow::showPvAIPage() {
    PvAIPage *pvaiPage = new PvAIPage(this);  // Create PvAI config page

    // Connect back button
    connect(pvaiPage, &PvAIPage::backToMenu, this, [this]() {
        stack->setCurrentWidget(currentMenu);
    });

    // Connect start button with selected difficulty and settings
    connect(pvaiPage, &PvAIPage::startPvAIGame, this, [this](const QString &difficulty, bool showHints, bool soundEnabled) {
        onStartGame(1, difficulty, showHints, soundEnabled, true);
    });

    // Connect settings icon
    connect(pvaiPage, &PvAIPage::settingsClicked, this, &MainWindow::openSettingsDialog);

    // Show page
    stack->addWidget(pvaiPage);
    stack->setCurrentWidget(pvaiPage);
}

/// @brief Shows the How-To-Play instructions screen.
void MainWindow::showInstructionsPage() {
    InstructionsPage *instrPage = new InstructionsPage(this);  // Create instructions page

    // Connect back button
    connect(instrPage, &InstructionsPage::backToMenu, this, [this]() {
        stack->setCurrentWidget(currentMenu);
    });

    // Connect settings icon
    connect(instrPage, &InstructionsPage::settingsClicked, this, &MainWindow::openSettingsDialog);

    // Show page
    stack->addWidget(instrPage);
    stack->setCurrentWidget(instrPage);
}

/// @brief Opens the in-game settings dialog for toggling music and sound.
void MainWindow::openSettingsDialog() {
    // Create settings dialog with current toggles
    SettingsDialog *dialog = new SettingsDialog(isMusicEnabled, isSoundEffectsEnabled, this);

    // Connect toggles to update states
    connect(dialog, &SettingsDialog::musicToggled, this, &MainWindow::toggleMusic);
    connect(dialog, &SettingsDialog::soundEffectsToggled, this, &MainWindow::toggleSoundEffects);

    dialog->exec();   // Show modally
    delete dialog;    // Clean up after closing
}

/// @brief Enables or mutes background music.
void MainWindow::toggleMusic(bool enabled) {
    isMusicEnabled = enabled;
    audioOutput->setMuted(!enabled);  // Mute or unmute based on toggle
}

/// @brief Enables or disables sound effects.
void MainWindow::toggleSoundEffects(bool enabled) {
    isSoundEffectsEnabled = enabled;
}

/// @brief Displays a scrollable page with game instructions and rules.
void MainWindow::showGameDescriptionPage() {
    GameDescriptionPage *descPage = new GameDescriptionPage(this);  // Create game info page

    // Connect back button
    connect(descPage, &GameDescriptionPage::backToMenu, this, [this]() {
        stack->setCurrentWidget(currentMenu);
    });

    // Show page
    stack->addWidget(descPage);
    stack->setCurrentWidget(descPage);
}