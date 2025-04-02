/**
 * @file Piece.h
 * @brief Represents a checkers piece on the game board.
 *
 * Each piece tracks its color, king status, position, and ID.
 * Handles click events and updates its icon on promotion/demotion.
 *
 * @author Lok Hui
 */

#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QDebug>

/**
 * @enum PieceColor
 * @brief Represents the team/color of a checkers piece.
 */
enum class PieceColor {
    Red,
    Black,
    Green,
    Yellow
};

/**
 * @class Piece
 * @brief Represents a single piece on the checkers board.
 * 
 * Handles color, king status, position, and interaction.
 */
class Piece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    PieceColor m_color;   // Piece's color/team
    bool m_isKing;        // Whether this piece is a king
    int m_row;            // Row on the board
    int m_col;            // Column on the board
    int m_id;             // Unique ID for this piece
    static int nextId;    // Static counter for assigning unique IDs

public:
    /**
     * @brief Constructor that creates a piece with color, status, and position.
     *        Automatically assigns a unique ID.
     * @param color The piece's color.
     * @param isKing Whether it's a king.
     * @param row Initial row.
     * @param col Initial column.
     * @param parent Optional graphics parent.
     */
    Piece(PieceColor color, bool isKing, int row, int col, QGraphicsItem *parent = nullptr);

    /// @brief Gets the color of the piece.
    PieceColor getColor() const { return m_color; }

    /// @brief Returns true if the piece is a king.
    bool isKing() const { return m_isKing; }

    /// @brief Gets the row on the board.
    int getRow() const { return m_row; }

    /// @brief Gets the column on the board.
    int getCol() const { return m_col; }

    /// @brief Gets the unique piece ID.
    int getId() const { return m_id; }

    /**
     * @brief Sets the piece's unique ID (used when cloning).
     * @param id The ID to assign.
     */
    void setId(int id) { m_id = id; }

    /**
     * @brief Sets the board position (row and column).
     * @param row The new row.
     * @param col The new column.
     */
    void setBoardPosition(int row, int col);

    /**
     * @brief Promotes this piece to a king.
     */
    void promoteToKing();

    /**
     * @brief Reverts king to regular piece.
     */
    void demoteFromKing();

private:
    /**
     * @brief Updates the piece icon (used after promotion or demotion).
     */
    void updatePixmap();

protected:
    /**
     * @brief Handles mouse clicks on the piece.
     * @param event The mouse event object.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif