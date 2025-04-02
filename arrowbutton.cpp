/**
 * @file arrowbutton.cpp
 * @brief Implements a styled QPushButton subclass for text or icon buttons.
 *
 * Provides two constructors for creating either a text-based or icon-based button,
 * each with consistent sizing and custom styling for hover and press states.
 * 
 * @author Lok Hui
 */

#include "arrowbutton.h"

ArrowButton::ArrowButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)

/**
* @brief Constructor for text-based buttons.
* @param text Button label.
* @param parent Optional parent widget.
*/    
{
    setFixedSize(60, 60);  // Set fixed size for consistency

    // Apply styling for text-based buttons
    setStyleSheet(R"(
        QPushButton {
            font-size: 32px;
            font-weight: bold;
            border: 2px solid #2c3e50;
            border-radius: 10px;
            background-color: #b0b3b6;
            color: white;
            padding: 5px;
        }
        QPushButton:hover {
            background-color: #9ca0a3;
            border-color: #3d566e;
        }
        QPushButton:pressed {
            background-color: #7f8386;
            border-color: #1f2d3a;
        }
    )");
}

/**
* @brief Constructor for icon-based buttons.
* @param icon Button icon.
* @param parent Optional parent widget.
*/
ArrowButton::ArrowButton(const QIcon &icon, QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(60, 60);         // Fixed size for visual consistency
    setIcon(icon);                // Set the icon
    setIconSize(QSize(48, 48));   // Icon size within button

    // Apply custom styling for icon-based buttons
    setStyleSheet(R"(
        QPushButton {
            background-color: #D2B48C;
            border: 2px solid #8B4513;
            border-radius: 10px;
        }
        QPushButton:hover {
            background-color: #c6a97d;
        }
        QPushButton:pressed {
            background-color: #b18c5e;
        }
    )");
}