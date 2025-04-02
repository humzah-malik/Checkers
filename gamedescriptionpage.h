/**
 * @file gamedescriptionpage.h
 * @brief Implements the UI for the game description screen.
 * 
 * Displays game instructions, objectives, and controls in a scrollable layout.
 * Includes a back button to return to the main menu.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef GAMEDESCRIPTIONPAGE_H
#define GAMEDESCRIPTIONPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

/**
 * @class GameDescriptionPage
 * @brief Displays an informative screen with instructions, controls, and features for the game.
 * 
 * Includes a scrollable text area and a back button that returns the user to the main menu.
 */
class GameDescriptionPage : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the GameDescriptionPage UI.
     * @param parent Optional parent widget.
     */
    explicit GameDescriptionPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the user clicks the back arrow button to return to the main menu.
     */
    void backToMenu();

private:
    QLabel *descriptionLabel;  // Label containing the long description text
    QPushButton *backButton;   // Button to navigate back to main menu
};

#endif // GAMEDESCRIPTIONPAGE_H