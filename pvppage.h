/**
 * @file pvppage.h
 * @brief Implements the UI for the Player vs Player selection screen in the Checkers game.
 * 
 * Provides options for choosing between 1v1 and 4-player multiplayer modes (with the latter currently not implemented).
 * Includes navigation buttons for going back, accessing settings, and exiting the application.
 *  
 * @author Humzah Zahid Malik
 */

#ifndef PVPPAGE_H
#define PVPPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>

/**
 * @class PvPPage
 * @brief UI page for selecting Player vs Player (PvP) game mode.
 * 
 * Allows the user to choose between 2-player and 4-player modes.
 */
class PvPPage : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor that sets up the PvP game mode selection UI.
     * @param parent Parent widget.
     */
    explicit PvPPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when user wants to return to main menu.
     */
    void backToMenu();

    /**
     * @brief Emitted when user starts a PvP game.
     * @param mode 2 = 2-player mode, 4 = 4-player mode.
     */
    void startPvPGame(int mode);

    /**
     * @brief Emitted when settings button is clicked.
     */
    void settingsClicked();

private slots:
    /**
     * @brief Called when 1v1 button is clicked.
     */
    void onOneVOneClicked();

    /**
     * @brief Called when 4-player button is clicked.
     */
    void onFourPlayerClicked();

private:
    QLabel *titleLabel;             // Title at top of page
    QPushButton *oneVOneButton;     // Button for 2-player mode
    QPushButton *fourPlayerButton;  // Button for 4-player mode
    QPushButton *backButton;        // Back navigation button
    QPushButton *exitButton;        // Exit app button
    QPushButton *settingsButton;    // Opens settings dialog

    QRadioButton *mode2RadioButton; // Optional radio for 2-player mode
    QRadioButton *mode4RadioButton; // Optional radio for 4-player mode
    QButtonGroup *modeButtonGroup;  // Groups the mode radio buttons
};

#endif // PVPPAGE_H