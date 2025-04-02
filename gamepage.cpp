/**
 * @file gamepage.cpp
 * @brief Implements the main in-game UI for a Checkers match.
 * 
 * Sets up the game layout, controls (undo, back, settings, exit), and 
 * displays turn indicators, undo counts, and game-over dialogs.
 * Handles user interactions and connects to CheckersBoard events.
 * 
 * @author Lok Hui
 */

#include "gamepage.h"
#include <QIcon>
#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "stylehelpers.h"
#include <QMessageBox>

/**
 * @brief Constructs the GamePage, setting up layout and connecting signals.
 */
GamePage::GamePage(QWidget *gameWidget, const QString &gameTitle, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *settingBar = new QHBoxLayout();
    QVBoxLayout *titleArea = new QVBoxLayout();
    QHBoxLayout *bottomBar = new QHBoxLayout();

    // -------------------- Back Button --------------------
    backButton = new ArrowButton(QIcon("./Icons/arrow_back.png"), this);
    backButton->setFixedSize(60, 60);

    // -------------------- Settings Button --------------------
    QPushButton *settingsButton = new QPushButton("", this);
    settingsButton->setIcon(QIcon("./Icons/setting.png"));
    settingsButton->setIconSize(QSize(48, 48));
    settingsButton->setFixedSize(60, 60);

    // -------------------- Exit Button --------------------
    exitButton = new QPushButton("Exit", this);
    exitButton->setFixedSize(130, 60);
    applyRetroButtonStyle(exitButton);

    // Button Connections
    connect(backButton, &QPushButton::clicked, this, [=]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(this->window()))
            mainWindow->playSelectSound();
        emit backToMenu();
    });

    connect(exitButton, &QPushButton::clicked, this, [=]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(this->window()))
            mainWindow->playSelectSound();
        emit exitApp();
    });

    connect(settingsButton, &QPushButton::clicked, this, [=]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(this->window()))
            mainWindow->playSelectSound();
        emit settingsClicked();
    });

    // -------------------- Title Label --------------------
    QLabel *titleLabel = new QLabel(gameTitle, this);
    titleLabel->setStyleSheet("font-family: 'Russo One'; font-size: 52px; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // -------------------- Turn Indicator --------------------
    turnIndicatorLabel = new QLabel("Black's Turn", this);
    turnIndicatorLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; background-color: white; "
        "color: black; padding: 5px; border: 1px solid black;"
    );
    turnIndicatorLabel->setAlignment(Qt::AlignCenter);

    // -------------------- Undo Labels --------------------
    redUndosLabel = new QLabel("Red's undos: 2", this);
    redUndosLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: red; background-color: white; border: 1px solid black; padding: 6px;");

    blackUndosLabel = new QLabel("Black's undos: 2", this);
    blackUndosLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: black; background-color: white; border: 1px solid black; padding: 6px;");

    QWidget *undoContainer = new QWidget(this);
    undoContainer->setStyleSheet("background-color: #003153;");
    QHBoxLayout *undoBar = new QHBoxLayout(undoContainer);
    undoBar->setContentsMargins(20, 8, 20, 8);
    undoBar->setSpacing(30);
    undoBar->addWidget(redUndosLabel);
    undoBar->addWidget(blackUndosLabel);

    // -------------------- Undo Button --------------------
    undoButton = new QPushButton("Undo", this);
    undoButton->setFixedSize(130, 60);
    applyRetroButtonStyle(undoButton);
    connect(undoButton, &QPushButton::clicked, this, [=]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(this->window()))
            mainWindow->playSelectSound();
        onUndoClicked();
    });

    // -------------------- Top Setting Bar --------------------
    settingBar->addStretch();
    settingBar->addWidget(settingsButton, 0, Qt::AlignRight);

    // -------------------- Title Area Stack --------------------
    titleArea->setAlignment(Qt::AlignCenter);
    titleArea->addWidget(titleLabel);
    titleArea->addSpacing(10);
    titleArea->addWidget(turnIndicatorLabel);
    titleArea->addSpacing(10);
    titleArea->addWidget(undoContainer, 0, Qt::AlignCenter);

    // -------------------- Game Board --------------------
    QWidget *boardWrapper = new QWidget(this);
    QVBoxLayout *boardLayout = new QVBoxLayout(boardWrapper);
    boardLayout->addWidget(gameWidget);
    boardLayout->setAlignment(Qt::AlignCenter);
    boardLayout->setContentsMargins(0, 0, 0, 0);
    boardWrapper->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // -------------------- Bottom Bar --------------------
    bottomBar->addWidget(backButton);
    bottomBar->addWidget(undoButton);
    bottomBar->addStretch();
    bottomBar->addWidget(exitButton);

    // -------------------- Assemble Layout --------------------
    layout->addLayout(settingBar);
    layout->addLayout(titleArea);
    layout->addSpacing(20);
    layout->addWidget(boardWrapper, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addLayout(bottomBar);
    setLayout(layout);

    // -------------------- Connect Game Events --------------------
    if (auto board = qobject_cast<CheckersBoard*>(gameWidget)) {
        connect(board, &CheckersBoard::gameEnded, this, [=]() {
            PieceColor winner = (board->getPieces(PieceColor::Red).isEmpty()) ? PieceColor::Black : PieceColor::Red;
            showGameOverDialog(winner);
        });
        connect(board, &CheckersBoard::turnChanged, this, &GamePage::updateTurnIndicator);
    }
}

/**
 * @brief Handles Undo button click.
 */
void GamePage::onUndoClicked()
{
    emit requestUndo();
}

/**
 * @brief Updates both players' undo labels (PvP).
 */
void GamePage::updateUndoLabels(int redLeft, int blackLeft)
{
    redUndosLabel->setText(QString("Red's undos: %1").arg(redLeft));
    blackUndosLabel->setText(QString("Black's undos: %1").arg(blackLeft));
}

/**
 * @brief Updates undo label for PvAI mode and hides red's label.
 */
void GamePage::updateUserUndoLabel(int remaining)
{
    redUndosLabel->hide();
    blackUndosLabel->setText(QString("Your undos: %1").arg(remaining));
}

/**
 * @brief Updates the displayed turn indicator based on player color.
 */
void GamePage::updateTurnIndicator(PieceColor newTurn)
{
    QString text = (newTurn == PieceColor::Red) ? "Red's Turn" : "Black's Turn";
    QString color = (newTurn == PieceColor::Red) ? "red" : "black";
    turnIndicatorLabel->setText(text);
    turnIndicatorLabel->setStyleSheet(QString(
        "font-size: 24px; font-weight: bold; background-color: white; "
        "color: %1; padding: 5px; border: 1px solid black;"
    ).arg(color));
}

/**
 * @brief Displays a dialog announcing the game winner with options to restart or return to menu.
 */
void GamePage::showGameOverDialog(PieceColor winner)
{
    QString winnerText = (winner == PieceColor::Red) ? "Red Wins!" : "Black Wins!";

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Game Over");
    dialog->setModal(true);
    dialog->setFixedSize(420, 300);

    dialog->setStyleSheet(R"(
        QDialog {
            background-color: #003153;
        }
        QLabel {
            color: #D2B48C;
            font-size: 28px;
            font-weight: bold;
            padding: 10px;
        }
        QPushButton {
            font-size: 16px;
            font-weight: bold;
            border: 2px solid #8B4513;
            border-radius: 10px;
            background-color: #fff2dc;
            color: #8B4513;
            padding: 6px 16px;
            min-width: 180px;
            min-height: 40px;
        }
        QPushButton:hover {
            background-color: #f5deb3;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel(winnerText, dialog);
    label->setAlignment(Qt::AlignCenter);

    QPushButton *playAgainButton = new QPushButton("Play Again", dialog);
    QPushButton *menuButton = new QPushButton("Return to Main Menu", dialog);

    layout->addWidget(label);
    layout->addSpacing(20);
    layout->addWidget(playAgainButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(menuButton, 0, Qt::AlignCenter);

    connect(playAgainButton, &QPushButton::clicked, dialog, [=]() {
        dialog->accept();
        emit playAgainRequested();
    });

    connect(menuButton, &QPushButton::clicked, dialog, [=]() {
        dialog->accept();
        emit backToMenu();
    });

    dialog->exec();
}