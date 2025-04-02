/**
 * @file gamedescriptionpage.cpp
 * @brief Implements the UI for the game description screen.
 * 
 * Displays game instructions, objectives, and controls in a scrollable layout.
 * Includes a back button to return to the main menu.
 * 
 * @author Humzah Zahid Malik
 */

#include "gamedescriptionpage.h"
#include "arrowbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QScrollArea>
#include <QApplication>
#include "mainwindow.h"

/**
 * @brief Constructs the full layout for the game description screen.
 * 
 * Adds a title, scrollable text area with game details, and a back button.
 */
GameDescriptionPage::GameDescriptionPage(QWidget *parent)
    : QWidget(parent)
{
    // -------------------- Title Label --------------------
    QLabel *title = new QLabel("Game Description", this);
    title->setStyleSheet("font-family: 'Russo One'; font-size: 52px; color: white;");
    title->setAlignment(Qt::AlignCenter);

    // -------------------- Description Content --------------------
    descriptionLabel = new QLabel(this);
    descriptionLabel->setText(
        "🎯 Objective:\n"
        "Capture all of your opponent's pieces or block them from making any legal moves.\n\n"

        "🎮 Gameplay Overview:\n"
        "• Classic 2-player board game played on an 8x8 grid.\n"
        "• Players take turns moving one piece at a time.\n"
        "• Pieces move diagonally and capture by jumping.\n"
        "• Reach the far side to promote a piece to a 👑 King, which can move both forward and backward.\n\n"

        "✨ Features in This Version:\n"
        "• Retro-themed design with smooth transitions.\n"
        "• Two exciting modes: Player vs Player and Player vs AI.\n"
        "• Sound effects and a built-in Undo system for added flexibility.\n\n"

        "🕹️ Controls:\n"
        "• Click on a piece to select it.\n"
        "• Valid move tiles will highlight automatically.\n"
        "• Click again on a highlighted square to move.\n\n"

        "Enjoy this polished digital version of Checkers!"
    );
    descriptionLabel->setStyleSheet("font-size: 20px; color: white;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // -------------------- Scroll Area --------------------
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(descriptionLabel);
    scrollArea->setStyleSheet("background-color: transparent; border: none;");

    // -------------------- Content Frame --------------------
    QFrame *contentFrame = new QFrame(this);
    contentFrame->setStyleSheet(
        "background-color: black;"
        "border: 2px solid white;"
        "border-radius: 10px;"
        "padding: 20px;"
    );
    QVBoxLayout *frameLayout = new QVBoxLayout(contentFrame);
    frameLayout->addWidget(scrollArea);
    contentFrame->setLayout(frameLayout);
    contentFrame->setFixedWidth(800);
    contentFrame->setFixedHeight(400);

    // -------------------- Back Button --------------------
    backButton = new ArrowButton(QIcon("./Icons/arrow_back.png"), this);
    backButton->setFixedSize(60, 60);

    // Connect back button click to signal with optional sound effect
    connect(backButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound();  // Optional: play selection sound
        emit backToMenu();  // Emit signal to go back
    });

    // -------------------- Layout Setup --------------------
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top row: back button on left
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(backButton);
    topBar->addStretch();

    // Center row: game title + content frame
    QHBoxLayout *centerRow = new QHBoxLayout();
    centerRow->addStretch();
    centerRow->addWidget(contentFrame);
    centerRow->addStretch();

    // Assemble all components
    mainLayout->addLayout(topBar);
    mainLayout->addWidget(title);
    mainLayout->addStretch();
    mainLayout->addLayout(centerRow);
    mainLayout->addStretch();

    setLayout(mainLayout);
}