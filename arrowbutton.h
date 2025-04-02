/**
 * @file arrowbutton.h
 * @brief Implements a styled QPushButton subclass for text or icon buttons.
 *
 * Provides two constructors for creating either a text-based or icon-based button,
 * each with consistent sizing and custom styling for hover and press states.
 * 
 * @author Lok Hui
 */

#ifndef ARROWBUTTON_H
#define ARROWBUTTON_H

#include <QPushButton>

/**
 * @class ArrowButton
 * @brief Custom QPushButton with either text or icon, styled for UI consistency.
 */
class ArrowButton : public QPushButton {
    Q_OBJECT

public:

    /**
     * @brief Constructor for text-based buttons.
     * @param text Button label.
     * @param parent Optional parent widget.
     */
    explicit ArrowButton(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief Constructor for icon-based buttons.
     * @param icon Button icon.
     * @param parent Optional parent widget.
     */
    explicit ArrowButton(const QIcon &icon, QWidget *parent = nullptr);
};

#endif // ARROWBUTTON_H