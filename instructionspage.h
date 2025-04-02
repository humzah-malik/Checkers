/**
 * @file instructionspage.h
 * @brief Implements the Game Instructions page UI.
 * 
 * Displays gameplay rules, controls, king mechanics, undo system, and settings info.
 * Includes a scrollable content section and a back button to return to the main menu.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef INSTRUCTIONSPAGE_H
#define INSTRUCTIONSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

/**
 * @class InstructionsPage
 * @brief Displays the rules, controls, and guidance for playing the Checkers game.
 *
 * This page provides a scrollable section explaining gameplay rules, king mechanics,
 * undo system, controls, and win conditions. Includes a back button for navigation.
 */
class InstructionsPage : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the InstructionsPage UI.
     * @param parent Optional parent widget.
     */
    explicit InstructionsPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the user clicks the back arrow to return to the menu.
     */
    void backToMenu();

    /**
     * @brief (Optional) Emitted when the user wants to access settings.
     */
    void settingsClicked();

private:
    QLabel *titleLabel;           // Page title label
    QLabel *instructionsLabel;    // Full text of instructions
    QPushButton *backButton;      // Arrow-style back button
};

#endif // INSTRUCTIONSPAGE_H