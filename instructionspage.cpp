/**
 * @file instructionspage.cpp
 * @brief Implements the Game Instructions page UI.
 * 
 * Displays gameplay rules, controls, king mechanics, undo system, and settings info.
 * Includes a scrollable content section and a back button to return to the main menu.
 * 
 * @author Humzah Zahid Malik
 */

#include "instructionspage.h"
#include "arrowbutton.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QApplication>

/**
 * @brief Constructs the instructions screen with title, scrollable content, and a back button.
 */
InstructionsPage::InstructionsPage(QWidget *parent) : QWidget(parent) {
    // -------------------- Title --------------------
    titleLabel = new QLabel("Game Instructions", this);
    titleLabel->setStyleSheet("font-family: 'Russo One'; font-size: 52px; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // -------------------- Instructions Content --------------------
    instructionsLabel = new QLabel(
        "📌 Basic Rules:\n"
        "• Players take alternating turns, moving one piece at a time.\n"
        "• Pieces move diagonally on dark squares and only forward (unless they are Kings).\n"
        "• A capture is made by jumping over an adjacent opponent’s piece into an empty square.\n"
        "• Multiple jumps in a single turn are allowed — and must be taken if available.\n\n"

        "👑 King Rules:\n"
        "• Reach the opposite end of the board to promote a piece to a King.\n"
        "• Kings can move and capture in both directions — forward and backward.\n\n"

        "🛑 Win Condition:\n"
        "• Win by capturing all of your opponent’s pieces or blocking all their valid moves.\n\n"

        "🔁 Undo System:\n"
        "• In PvP mode, each player has 2 undos per game.\n"
        "• In PvAI mode, you (the human) have 2 undos.\n"
        "• Press the 'Undo' button to revert the last move.\n\n"

        "⚙️ Settings:\n"
        "• Use the 'Settings' menu to toggle sound effects and background music anytime.\n\n"

        "🕹️ Controls:\n"
        "• Click on a piece to select it — valid moves will highlight automatically.\n"
        "• Click again on a highlighted square to move.\n\n",
        this
    );
    instructionsLabel->setStyleSheet("font-size: 20px; color: white;");
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // -------------------- Scroll Area --------------------
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(instructionsLabel);
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
    connect(backButton, &QPushButton::clicked, this, [=]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(this->window()))
            mainWindow->playSelectSound();
        emit backToMenu();
    });

    // -------------------- Top Bar --------------------
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(backButton);
    topBar->addStretch();

    // -------------------- Center Layout --------------------
    QHBoxLayout *centerRow = new QHBoxLayout();
    centerRow->addStretch();
    centerRow->addWidget(contentFrame);
    centerRow->addStretch();

    // -------------------- Main Layout --------------------
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBar);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(130); // Push content downward
    mainLayout->addLayout(centerRow);
    mainLayout->addStretch();    // Allow vertical breathing room

    setLayout(mainLayout);
}