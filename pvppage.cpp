/**
 * @file pvppage.cpp
 * @brief Implements the UI for the Player vs Player selection screen in the Checkers game.
 * 
 * Provides options for choosing between 1v1 and 4-player multiplayer modes (with the latter currently not implemented).
 * Includes navigation buttons for going back, accessing settings, and exiting the application.
 *  
 * @author Humzah Zahid Malik
 */

#include "pvppage.h"
#include "arrowbutton.h" 
#include <QApplication> 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include "stylehelpers.h"

/**
 * @brief Constructs the PvP mode selection page.
 * @param parent Parent widget
 */
PvPPage::PvPPage(QWidget *parent) : QWidget(parent) {
    // Title setup
    titleLabel = new QLabel("Player vs Player", this);
    titleLabel->setStyleSheet("font-family: 'Russo One'; font-size: 52px; color: white;");

    // 1v1 mode button
    oneVOneButton = new QPushButton("1v1", this);
    oneVOneButton->setFixedSize(320, 80);
    applyRetroButtonStyle(oneVOneButton);

    // 4-player mode button (not implemented yet)
    //fourPlayerButton = new QPushButton("4-Player Multiplayer (Coming Soon)", this);
    //fourPlayerButton->setFixedSize(320, 80);
    //applyRetroButtonStyle(fourPlayerButton);

    // Back button (left arrow)
    backButton = new ArrowButton(QIcon("./Icons/arrow_back.png"), this);
    backButton->setFixedSize(60, 60);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit backToMenu();
    });

    // Settings button (gear icon)
    settingsButton = new QPushButton("", this);
    settingsButton->setIcon(QIcon("./Icons/setting.png"));
    settingsButton->setIconSize(QSize(48,48));
    settingsButton->setFixedSize(60,60);
    connect(settingsButton, &QPushButton::clicked, this, [this]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit settingsClicked();
    });

    // Exit button
    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(130, 60);
    applyRetroButtonStyle(exitButton);
    connect(exitButton, &QPushButton::clicked, this, [this]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        qApp->quit(); // Close the application
    });

    // Connect mode buttons
    connect(oneVOneButton, &QPushButton::clicked, this, [this]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit startPvPGame(2);  // 2-player mode
    });

    connect(fourPlayerButton, &QPushButton::clicked, this, [this]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();
        emit startPvPGame(4);  // 4-player placeholder
    });

    // Layout for mode selection
    QVBoxLayout *modeLayout = new QVBoxLayout();
    modeLayout->setAlignment(Qt::AlignCenter);
    modeLayout->addWidget(oneVOneButton);
    modeLayout->addSpacing(20);
    modeLayout->addWidget(fourPlayerButton);

    // Top bar with back/settings/exit
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(backButton);
    topBar->addStretch();
    topBar->addWidget(settingsButton);
    topBar->addSpacing(20);
    topBar->addWidget(exitButton);

    // Final main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBar);
    mainLayout->addStretch(); // Push content to center
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(modeLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

/**
 * @brief Handles click on 1v1 button (not currently used, logic in lambda).
 */
void PvPPage::onOneVOneClicked() {
    emit startPvPGame(2); // Emit for 2-player mode
}

/**
 * @brief Handles click on 4-player button (not currently used, logic in lambda).
 */
void PvPPage::onFourPlayerClicked() {
    emit startPvPGame(4); // Emit for 4-player mode
}