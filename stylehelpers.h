/**
 * @file stylehelpers.h
 * @brief Utility for applying retro styles to QPushButtons.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef STYLEHELPERS_H
#define STYLEHELPERS_H

#include <QPushButton>

/**
 * @brief Applies a retro-themed style to a QPushButton.
 * 
 * This function sets custom font, colors, borders, and hover/pressed effects 
 * to give the button a vintage arcade feel.
 * 
 * @param button Pointer to the QPushButton to style.
 */
inline void applyRetroButtonStyle(QPushButton *button) {
    // Set stylesheet for normal, hover, and pressed states
    button->setStyleSheet(R"(
        QPushButton {
            font-size: 32px;              /* Large font for retro feel */
            font-weight: bold;            /* Bold text */
            border: 2px solid #8B4513;    /* Brown border */
            border-radius: 10px;          /* Rounded corners */
            background-color: #D2B48C;    /* Light tan background */
            color: #8B4513;               /* Brown text */
            padding: 10px;                /* Inner spacing */
        }
        QPushButton:hover {
            background-color: #c6a97d;    /* Slightly darker on hover */
        }
        QPushButton:pressed {
            background-color: #b18c5e;    /* Even darker on press */
        }
    )");
}

#endif // STYLEHELPERS_H