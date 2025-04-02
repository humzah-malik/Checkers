/**
 * @file checkersboard.h
 * @brief Implements the CheckersBoard and BoardSquare classes for managing the checkers game UI and logic.
 * 
 * This file handles the visual representation of the checkers board, move validation, captures, king promotion, 
 * turn switching, highlighting valid moves, undo functionality, and win condition checking.
 * 
 * It also defines BoardSquare for individual cells and manages user interactions via mouse clicks.
 * 
 * @author Lok Hui, Humzah Zahid Malik, Krish Bhavin Patel
 */

#ifndef CHECKERSBOARD_H
#define CHECKERSBOARD_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include <stack>
#include "piece.h"  // Defines Piece and PieceColor

/**
 * @struct MoveRecord
 * @brief Stores a complete move history record for undo functionality.
 */
struct MoveRecord {
    int pieceId;
    int oldRow, oldCol;
    int newRow, newCol;
    bool wasPromoted;
    bool captureOccurred;
    int capturedPieceId;
    int capturedRow, capturedCol;
    bool capturedWasKing;
    PieceColor capturedColor;
};

static const int BOARD_SIZE = 8;

/**
 * @class BoardSquare
 * @brief Represents an individual square on the checkers board.
 */
class BoardSquare : public QGraphicsRectItem {
public:
    /**
     * @brief Constructs a board square at a specific location.
     */
    BoardSquare(int row, int col, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);

    int getRow() const;               // Returns the row index of the square.
    int getCol() const;               // Returns the column index of the square.
    void resetColor();                // Resets square color to default.

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_row;                        // Row index of the square.
    int m_col;                        // Column index of the square.
};

/**
 * @class CheckersBoard
 * @brief Manages the visual board and game logic for Checkers.
 */
class CheckersBoard : public QGraphicsView {
    Q_OBJECT

public:
    explicit CheckersBoard(QWidget *parent = nullptr);      // Main constructor.

    CheckersBoard *clone() const;                           // Clones the board state.

    Piece* getPieceAt(int row, int col);                    // Returns piece at a specific location.
    Piece* getPieceById(int id);                            // Finds a piece by its ID.

    void handleMove(Piece *piece, int newRow, int newCol);  // Moves a piece and updates state.
    bool isValidMove(Piece *piece, int newRow, int newCol); // Checks if a move is legal.
    bool isAnyCaptureAvailable();                           // Checks if any piece has a capture.
    bool hasValidMoves(PieceColor color);                   // Checks if a color has any valid moves.
    bool isCaptureAvailable(Piece *piece);                  // Checks if a capture is possible for a piece.
    bool isCaptureMove(Piece *piece, int newRow, int newCol); // Checks if a move is a capturing move.

    void setSelectedPiece(Piece *piece);                    // Updates the currently selected piece.
    void switchTurn();                                      // Switches the current turn.
    void forceTurn(PieceColor color);                       // Forcefully sets the current turn.

    Piece *getSelectedPiece() const;                        // Gets the selected piece.
    PieceColor getCurrentTurn() const;                      // Gets whose turn it is.

    bool checkwin(PieceColor color);                        // Checks if a color has won.

    QList<Piece*> getPieces(PieceColor color);              // Returns all pieces of a given color.
    QList<Piece*> getAIPieces();                            // Returns AI pieces.
    QList<Piece*> getOpponentPieces();                      // Returns pieces of the non-current turn.

    void undoLastMove();                                    // Undoes the most recent move.
    void highlightValidMoves(Piece* piece);                 // Highlights valid squares for a given piece.
    void clearHighlightedSquares();                         // Clears highlighted squares.

signals:
    void moveCompleted();         // Emitted when a move is completed.
    void gameCheck();             // Emitted when a check state occurs.
    void turnChanged(PieceColor newTurn); // Emitted when the turn changes.
    void pieceCaptured();         // Emitted when a piece is captured.
    void piecePromoted();         // Emitted when a piece is promoted.
    void gameEnded();             // Emitted when the game ends.

private:
    QGraphicsScene *m_scene;                // Graphics scene for rendering.
    Piece *selectedPiece;                   // Currently selected piece.
    PieceColor currentTurn = PieceColor::Black; // Whose turn it is.
    std::stack<MoveRecord> moveHistory;     // Stack for undo functionality.
    QList<BoardSquare*> m_highlightedSquares;// Squares currently highlighted.
    bool m_captureMade = false;             // Flag if a capture was made.

    /**
     * @brief Checks if a piece belongs to the current player.
     */
    bool isPiecesTurn(Piece* piece);

    void initializeBoard();                 // Sets up the board squares.
    void initializePieces();                // Places the pieces on the board.
};

#endif // CHECKERSBOARD_H