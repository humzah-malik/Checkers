/**
 * @file checkersmenu.cpp
 * @brief Implements the main menu UI for the Checkers game.
 * 
 * Handles game mode selection (PvP, PvAI), difficulty adjustment,
 * and links to instructions, settings, and game description pages.
 * 
 * @author Lok Hui
 */

#include "checkersmenu.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QIcon>
#include "mainwindow.h"
#include "stylehelpers.h"

/**
 * @brief Constructor for the CheckersMenu.
 * 
 * Initializes difficulty list and sets up the UI layout.
 */
CheckersMenu::CheckersMenu(QWidget *parent) :
    QWidget(parent),
    difficultyIndex(0),
    difficulties({"Easy", "Medium", "Hard"})
{
    setupUI();
    updateDifficultyLabel();
}

/**
 * @brief Builds and styles the full UI for the checkers menu.
 */
void CheckersMenu::setupUI() {
    // Title label
    titleLabel = new QLabel("Checkers", this);
    titleLabel->setStyleSheet("font-family: 'Russo One'; font-size: 56px; color: white;");

    // Settings button (gear icon)
    settingsButton = new QPushButton("", this);
    settingsButton->setIcon(QIcon("./Icons/setting.png"));
    settingsButton->setIconSize(QSize(48, 48));
    settingsButton->setFixedSize(60, 60);
    connect(settingsButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit settingsRequested();
    });

    // Exit button
    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(130, 60);
    applyRetroButtonStyle(exitButton);
    connect(exitButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit exitAppRequested();
    });

    // Top bar layout
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->addStretch();
    topBarLayout->addWidget(settingsButton, 0, Qt::AlignRight);
    topBarLayout->addSpacing(20);
    topBarLayout->addWidget(exitButton, 0, Qt::AlignRight);

    // Game mode buttons
    QPushButton *pvpButton = new QPushButton("PvP", this);
    QPushButton *pvaiButton = new QPushButton("PvAI", this);
    QPushButton *instructionsButton = new QPushButton("Game Instructions", this);

    applyRetroButtonStyle(pvpButton);
    applyRetroButtonStyle(pvaiButton);
    applyRetroButtonStyle(instructionsButton);

    pvpButton->setFixedSize(320, 80);
    pvaiButton->setFixedSize(320, 80);
    instructionsButton->setFixedSize(320, 80);

    // Connect game mode buttons
    connect(pvpButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit pvpSelected();
    });

    connect(pvaiButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit pvaiSelected();
    });

    connect(instructionsButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit instructionsSelected();
    });

    // Layout for game mode buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(pvpButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(pvaiButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(instructionsButton);

    // Game description button
    QPushButton *descButton = new QPushButton("Game Description", this);
    descButton->setFixedSize(320, 80);
    applyRetroButtonStyle(descButton);
    connect(descButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        mainWindow->showGameDescriptionPage();
    });

    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(descButton);

    // Full layout assembly
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBarLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(30);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

/**
 * @brief Updates the difficulty label based on the current index.
 */
void CheckersMenu::updateDifficultyLabel() {
    if (difficultyIndex < 0 || difficultyIndex >= difficulties.size()) {
        difficultyIndex = 0;
    }

    // Placeholder for when difficulty UI is shown later
    if (difficultyLabel)
        difficultyLabel->setText(difficulties[difficultyIndex]);
}

/**
 * @brief Decreases difficulty level and updates label.
 */
void CheckersMenu::minusDifficultySlot() {
    if (difficultyIndex == 0) {
        difficultyIndex = difficulties.size() - 1;
    } else {
        difficultyIndex--;
    }
    updateDifficultyLabel();
}

/**
 * @brief Increases difficulty level and updates label.
 */
void CheckersMenu::addDifficultySlot() {
    difficultyIndex = (difficultyIndex + 1) % difficulties.size();
    updateDifficultyLabel();
}

/**
 * @brief Placeholder slot to react to game mode changes.
 */
void CheckersMenu::updateGameMode() {
}

/**
 * @brief Emits the startGame signal with current settings.
 * 
 * Placeholder implementation for future integration.
 */
void CheckersMenu::onStartClicked() {
    int numPlayers = 1; // placeholder
    QString currentDifficulty = difficulties[difficultyIndex];
    bool showHints = false;
    bool soundEnabled = false;
    bool aiEnabled = true;

    qDebug() << "[CheckersMenu] Start clicked!"
             << "Players =" << numPlayers
             << "Difficulty =" << currentDifficulty
             << "Hints =" << showHints
             << "Sound =" << soundEnabled
             << "AI Enabled = " << aiEnabled;

    emit startGameSignaled(numPlayers, currentDifficulty, showHints, soundEnabled, aiEnabled);
}