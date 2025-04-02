/**
 * @file frontpage.h
 * @brief Implements the landing screen for the Checkers game.
 * 
 * Displays the game title and a styled Start button.
 * Connects button interaction to the main window logic.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef FRONTPAGE_H
#define FRONTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

/**
 * @class FrontPage
 * @brief Represents the main entry UI screen with a title and Start button.
 *
 * This class displays the Checkers title and a retro-styled Start button.
 * Emits signals when the Start button is clicked or when the user wants to exit.
 */
class FrontPage : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout *layout;          ///< Main vertical layout
    QLabel *logo;                 ///< Logo label displaying the game title
    QPushButton *checkersButton;  ///< Button to start the Checkers game
    QPushButton *game2Btn;        ///< Placeholder for a second game (currently unused)
    QPushButton *game3Btn;        ///< Placeholder for a third game (currently unused)
    QPushButton *game4Btn;        ///< Placeholder for a fourth game (currently unused)
    QPushButton *exitButton;      ///< Exit button (currently commented out)

public:
    /**
     * @brief Constructor for FrontPage UI.
     * @param parent Optional parent QWidget.
     */
    explicit FrontPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the Checkers "Start" button is clicked.
     */
    void checkersClicked();

    /**
     * @brief Emitted when user requests to quit the application.
     */
    void exitAppRequested();
};

#endif