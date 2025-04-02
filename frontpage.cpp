/**
 * @file frontpage.cpp
 * @brief Implements the landing screen for the Checkers game.
 * 
 * Displays the game title and a styled Start button.
 * Connects button interaction to the main window logic.
 * 
 * @author Humzah Zahid Malik
 */

#include "frontpage.h"
#include <QApplication>
#include "stylehelpers.h"

/**
 * @brief Constructs the front page UI with title and Start button.
 * 
 * Applies retro styles, sets up layout, and connects signals.
 */
FrontPage::FrontPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);  // Main vertical layout for the page

    // Create and style the logo label
    logo = new QLabel("Checkers", this);
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet("font-family: 'Russo One'; font-size: 64px; color: white;");

    // Create and configure the Start button
    checkersButton = new QPushButton("Start", this);
    checkersButton->setFixedSize(220, 60);
    applyRetroButtonStyle(checkersButton);  // Apply retro appearance

    // Connect button click to custom signal
    connect(checkersButton, &QPushButton::clicked, this, &FrontPage::checkersClicked);

    // Center the Start button within its own layout
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(checkersButton);

    // (Optional) Setup for exit button — currently commented out
    /*
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(130, 60);
    applyRetroButtonStyle(exitButton);

    connect(exitButton, &QPushButton::clicked, this, [this]() {
        emit exitAppRequested();
    });
    bottomLayout->addWidget(exitButton, 0, Qt::AlignRight);
    */

    // Assemble the page layout
    layout->addStretch();            // Push content toward center
    layout->addWidget(logo);         // Game title at top
    layout->addSpacing(80);          // Spacing between title and button
    layout->addLayout(buttonLayout); // Centered Start button
    layout->addStretch();            // Push content toward center
    //layout->addLayout(bottomLayout); // Optional exit button
    setLayout(layout);
}
