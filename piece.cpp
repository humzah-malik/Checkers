/**
 * @file Piece.cpp
 * @brief Represents a checkers piece on the game board.
 *
 * Each piece tracks its color, king status, position, and ID.
 * Handles click events and updates its icon on promotion/demotion.
 *
 * @author Lok Hui
 */

#include "piece.h"
#include "checkersboard.h"

static const int PIECE_SIZE = 50; // Size for scaling piece image
int Piece::nextId = 1;            // Initialize static ID counter

/**
 * @brief Constructs a Piece with given color, king status, and board position.
 *        Assigns a unique ID and sets its visual representation.
 */
Piece::Piece(PieceColor color, bool isKing, int row, int col, QGraphicsItem *parent)
    : QObject(),
      QGraphicsPixmapItem(parent),
      m_color(color),
      m_isKing(isKing),
      m_row(row),
      m_col(col)
{
    setAcceptHoverEvents(true);                     // Allow hover effects
    setAcceptedMouseButtons(Qt::LeftButton);        // Only accept left click

    m_id = nextId++;                                // Assign unique ID

    updatePixmap();                                 // Set correct image
}

/**
 * @brief Updates the board coordinates of the piece.
 */
void Piece::setBoardPosition(int row, int col)
{
    m_row = row;   // Update row
    m_col = col;   // Update column
}

/**
 * @brief Promotes the piece to a king and updates icon.
 */
void Piece::promoteToKing()
{
    if (!m_isKing) {
        m_isKing = true;        // Set king flag
        updatePixmap();         // Change icon
    }
}

/**
 * @brief Handles mouse click on the piece for selection logic.
 */
void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Try to get the CheckersBoard this piece belongs to
    CheckersBoard *board = dynamic_cast<CheckersBoard *>(scene()->views().first());
    if (board) {
        // Check if this piece matches the current turn's color
        if (m_color == board->getCurrentTurn()) {
            if (board->getSelectedPiece() == this) {
            } else {
                board->setSelectedPiece(this);  // Select this piece
            }
        } else {
        }
    }

    QGraphicsPixmapItem::mousePressEvent(event); // Call base class
}

/**
 * @brief Updates the pixmap/icon of the piece based on color and king status.
 */
void Piece::updatePixmap()
{
    QString path;

    // Determine path based on color and king status
    if (m_color == PieceColor::Red)
        path = m_isKing ? "./Icons/red_king.png" : "./Icons/red_piece.png";
    else if (m_color == PieceColor::Black)
        path = m_isKing ? "./Icons/black_king.png" : "./Icons/black_piece.png";
    else if (m_color == PieceColor::Green)
        path = m_isKing ? "./Icons/green_king.png" : "./Icons/green_piece.png";
    else if (m_color == PieceColor::Yellow)
        path = m_isKing ? "./Icons/yellow_king.png" : "./Icons/yellow_piece.png";

    // Load and apply the image
    QPixmap pix(path);
    if (!pix.isNull()) {
        setPixmap(pix.scaled(
            PIECE_SIZE,
            PIECE_SIZE,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}

/**
 * @brief Demotes a king back to a regular piece.
 */
void Piece::demoteFromKing()
{
    m_isKing = false;   // Clear king flag
    updatePixmap();     // Refresh icon
}