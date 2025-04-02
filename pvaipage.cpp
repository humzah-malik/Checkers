/**
 * @file pvaipage.cpp
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

#include "pvaipage.h"
#include "arrowbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include <QApplication>
#include "stylehelpers.h"

/**
 * @brief Constructor for PvAIPage. Initializes UI elements and connects signals.
 */
PvAIPage::PvAIPage(QWidget *parent)
    : QWidget(parent), difficultyIndex(0), difficulties({"Easy", "Medium", "Hard"})
{
    // Title at top
    titleLabel = new QLabel("Player vs AI", this);
    titleLabel->setStyleSheet("font-family: 'Russo One'; font-size: 52px; color: white;");

    // Difficulty controls and label
    decreaseButton = new ArrowButton("-", this);
    increaseButton = new ArrowButton("+", this);
    difficultyLabel = new QLabel(difficulties[difficultyIndex], this);
    difficultyLabel->setStyleSheet("font-size: 32px; color: white;");
    difficultyLabel->setAlignment(Qt::AlignCenter);
    difficultyLabel->setFixedWidth(160);

    // Horizontal layout for difficulty row
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->setAlignment(Qt::AlignCenter);
    difficultyLayout->addWidget(decreaseButton);
    difficultyLayout->addWidget(difficultyLabel);
    difficultyLayout->addWidget(increaseButton);

    // Connect "-" button with audio and logic
    connect(decreaseButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        onDecreaseDifficulty();
    });

    // Connect "+" button with audio and logic
    connect(increaseButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        onIncreaseDifficulty();
    });

    // Start game button
    startButton = new QPushButton("Start", this);
    startButton->setFixedSize(130, 60);
    applyRetroButtonStyle(startButton);

    // Connect Start button with sound + logic
    connect(startButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        onStartClicked();
    });

    // Back button (arrow icon)
    backButton = new ArrowButton(QIcon("./Icons/arrow_back.png"), this);
    backButton->setFixedSize(60,60);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit backToMenu();
    });

    // Settings button (gear icon)
    settingsButton = new QPushButton("", this);
    settingsButton->setIcon(QIcon("./Icons/setting.png"));
    settingsButton->setIconSize(QSize(48,48));
    settingsButton->setFixedSize(60,60);
    connect(settingsButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit settingsClicked();
    });

    // Exit button to quit app
    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(130,60);
    applyRetroButtonStyle(exitButton);
    connect(exitButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        qApp->quit();
    });

    // Top layout (back, settings, exit)
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(backButton);
    topBar->addStretch();
    topBar->addWidget(settingsButton);
    topBar->addSpacing(20);
    topBar->addWidget(exitButton);

    // Main layout (top bar, title, difficulty, start)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBar);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(difficultyLayout);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(startButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

/**
 * @brief Decreases the difficulty index and updates label.
 */
void PvAIPage::onDecreaseDifficulty() {
    // Loop around to last if at beginning
    if (difficultyIndex == 0)
        difficultyIndex = difficulties.size() - 1;
    else
        difficultyIndex--;

    difficultyLabel->setText(difficulties[difficultyIndex]);
}

/**
 * @brief Increases the difficulty index and updates label.
 */
void PvAIPage::onIncreaseDifficulty() {
    // Wraps back to 0 if end reached
    difficultyIndex = (difficultyIndex + 1) % difficulties.size();
    difficultyLabel->setText(difficulties[difficultyIndex]);
}

/**
 * @brief Emits start signal with selected settings.
 */
void PvAIPage::onStartClicked() {
    // Hardcoded settings
    bool showHints = false;
    bool soundEnabled = false;

    // Emit signal to begin PvAI match
    emit startPvAIGame(difficulties[difficultyIndex], showHints, soundEnabled);
}