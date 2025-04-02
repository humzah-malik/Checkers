/**
 * @file checkersboard.cpp
 * @brief Implements the CheckersBoard and BoardSquare classes for managing the checkers game UI and logic.
 * 
 * This file handles the visual representation of the checkers board, move validation, captures, king promotion, 
 * turn switching, highlighting valid moves, undo functionality, and win condition checking.
 * 
 * It also defines BoardSquare for individual cells and manages user interactions via mouse clicks.
 * 
 * @author Lok Hui, Humzah Zahid Malik, Krish Bhavin Patel
 */

#include "checkersboard.h"
#include "piece.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>

// Cell size for board layout
static const int CELL_SIZE = 58;

/**
 * @class BoardSquare
 * @brief Represents an individual square on the checkers board.
 */
BoardSquare::BoardSquare(
    int row, 
    int col, 
    qreal x, 
    qreal y, 
    qreal w, 
    qreal h, 
    QGraphicsItem* parent)
    : QGraphicsRectItem(x, y, w, h, parent),  // Init base QGraphicsRectItem with position and size
      m_row(row),                             // Store row index
      m_col(col)                              // Store column index
{
    setAcceptHoverEvents(true);               // Enable hover events for this square
    setAcceptedMouseButtons(Qt::LeftButton);  // Only respond to left mouse clicks
}

// Returns the row index of the square
int BoardSquare::getRow() const {
    return m_row;
}

// Returns the column index of the square
int BoardSquare::getCol() const {
    return m_col;
}

// Resets square color based on its board position
void BoardSquare::resetColor() {
    // Use alternating colors for checkered pattern
    if ((m_row + m_col) % 2 == 0)
        setBrush(QColor(139, 69, 19));        // Dark wood color
    else
        setBrush(QColor(222, 184, 135));      // Light wood color
}

// Handles square clicks and triggers a move if valid
void BoardSquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Gets the CheckersBoard from the current scene
    CheckersBoard *board = dynamic_cast<CheckersBoard *>(scene()->views().first());
    if (board) {
        
        // Gets the currently selected piece from the board
        Piece *selectedPiece = board->getSelectedPiece();
        if (!selectedPiece) {
            return;
        }

        // If the square clicked is the same as the piece’s current square, do nothing
        if (selectedPiece->getRow() == m_row && selectedPiece->getCol() == m_col) {
            return;
        }

        // Attempt to move the selected piece to the clicked square
        board->handleMove(selectedPiece, m_row, m_col);
    }

    // Call base class handler
    QGraphicsRectItem::mousePressEvent(event);
}

/**
 * @class CheckersBoard
 * @brief Manages the visual board and game logic for Checkers.
 */
CheckersBoard::CheckersBoard(QWidget *parent) 
    : QGraphicsView(parent), selectedPiece(nullptr)
{
    // Create a new graphics scene for the board
    m_scene = new QGraphicsScene(this);

    // Set the scene size to fit the entire board
    m_scene->setSceneRect(0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    setScene(m_scene);  // Attach scene to the view

    // Initialize board layout and place pieces
    initializeBoard();
    initializePieces();

    setRenderHint(QPainter::Antialiasing);           // Smooth out graphics rendering
    setFixedSize(BOARD_SIZE * CELL_SIZE + 2, BOARD_SIZE * CELL_SIZE + 2); // Set fixed size of the view
}


/**
 * @brief Handles the movement of a piece to a new position.
 * 
 * Validates the move, performs captures, handles promotion, stores the move in history for undo, 
 * and emits relevant signals.
 * 
 * @param piece Pointer to the piece being moved.
 * @param newRow Target row to move the piece to.
 * @param newCol Target column to move the piece to.
 */
void CheckersBoard::handleMove(Piece *piece, int newRow, int newCol)
{

    // Abort if no piece is passed
    if (!piece) {
        return;
    }

    // Abort if the selected piece is not the current turn's color
    if (piece->getColor() != currentTurn) {
        return;
    }

    // Store old position of the piece
    int oldRow = piece->getRow();
    int oldCol = piece->getCol();

    // Init capture-related tracking
    m_captureMade = false;
    int capturedPieceId = -1;
    int capturedRow = -1, capturedCol = -1;
    bool capturedWasKing = false;
    PieceColor capturedColor;

    // Check if move is valid
    if (isValidMove(piece, newRow, newCol)) {
        // Check if this move is a capture (jump of 2 squares)
        if (abs(newRow - oldRow) == 2 && abs(newCol - oldCol) == 2) {
            int midRow = (oldRow + newRow) / 2;
            int midCol = (oldCol + newCol) / 2;

            // Looks for a capturable opponent piece at the midpoint
            for (QGraphicsItem *item : m_scene->items()) {
                Piece *midPiece = dynamic_cast<Piece *>(item);
                if (midPiece && midPiece->getRow() == midRow && midPiece->getCol() == midCol) {
                    if (midPiece->getColor() != piece->getColor()) {
                        m_captureMade = true;
                        capturedPieceId = midPiece->getId();
                        capturedRow = midRow;
                        capturedCol = midCol;
                        capturedWasKing = midPiece->isKing();
                        capturedColor = midPiece->getColor();

                        // Remove from scene
                        m_scene->removeItem(midPiece);
                        delete midPiece;

                        // Trigger capture event
                        emit pieceCaptured();
                        break;
                    }
                }
            }
        }

        // Moves piece to new position
        piece->setBoardPosition(newRow, newCol);

        // Checks for promotion to king
        bool wasPromoted = false;
        if (!piece->isKing()) {
            if ((piece->getColor() == PieceColor::Red && newRow == BOARD_SIZE - 1) ||
                (piece->getColor() == PieceColor::Black && newRow == 0)) {
                wasPromoted = true;
            }
        }

        // Saves move for undo
        MoveRecord record;
        record.pieceId = piece->getId();
        record.oldRow = oldRow;
        record.oldCol = oldCol;
        record.newRow = newRow;
        record.newCol = newCol;
        record.wasPromoted = wasPromoted;
        record.captureOccurred = m_captureMade;
        record.capturedPieceId = capturedPieceId;
        record.capturedRow = capturedRow;
        record.capturedCol = capturedCol;
        record.capturedWasKing = capturedWasKing;
        if (m_captureMade)
            record.capturedColor = capturedColor;
        moveHistory.push(record);

        // Sets new visual position
        qreal x = newCol * CELL_SIZE + (CELL_SIZE - piece->pixmap().width()) / 2.0;
        qreal y = newRow * CELL_SIZE + (CELL_SIZE - piece->pixmap().height()) / 2.0;
        piece->setPos(x, y);

        // Removes old highlights
        clearHighlightedSquares();

        // Handles king promotion
        if (!piece->isKing()) {
            if (piece->getColor() == PieceColor::Red && newRow == BOARD_SIZE - 1) {
                piece->promoteToKing();
                emit piecePromoted();
            }
            else if (piece->getColor() == PieceColor::Black && newRow == 0) {
                piece->promoteToKing();
                emit piecePromoted();
            }
        }

        // Checks if another capture is available with the same piece
        if (m_captureMade && isCaptureAvailable(piece)) {
            selectedPiece = piece;
            highlightValidMoves(piece);
            if (piece->getColor() == PieceColor::Red) {
                // Red triggers move complete after multi-capture
                emit moveCompleted();
            }
            // Don’t switch turn yet
            return;
        }

    } else {
        // Invalid move, abort
        return;
    }

    // Clears selected piece and switch turn
    selectedPiece = nullptr;
    switchTurn();

    // Notifies rest of system about turn completion
    emit moveCompleted();
    emit gameCheck();

    // Resets capture flag
    m_captureMade = false;
}

/**
 * @brief Checks if the given color has any valid move available.
 * 
 * Looks for either capture moves or normal moves for all pieces of the given color.
 * 
 * @param color The color to check moves for (Red or Black).
 * @return true if at least one valid move is available, false otherwise.
 */
bool CheckersBoard::hasValidMoves(PieceColor color)
{
    // Loops through all items in the scene
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece *>(item);

        // Skips if not a piece or not the right color
        if (!piece || piece->getColor() != color)
            continue;
        
        int row = piece->getRow();
        int col = piece->getCol();

        // If any capture is possible, returns true
        if (isCaptureAvailable(piece))
            return true;

        if (piece->isKing()) {
            // Kings can move in any diagonal direction
            int directions[] = {-1, 1};
            for (int dr : directions) {
                for (int dc : directions) {
                    if (isValidMove(piece, row + dr, col + dc))
                        return true;
                }
            }
        } else {
            // Regular pieces can only move forward (based on color)
            int forwardDir = (color == PieceColor::Red) ? 1 : -1;
            int dcOptions[] = {-1, 1};
            for (int dc : dcOptions) {
                if (isValidMove(piece, row + forwardDir, col + dc))
                    return true;
            }
        }
    }

    // No valid move found
    return false;
}

/**
 * @brief Checks if a given move for a piece is valid.
 * 
 * Handles basic move rules, prevents backward moves for non-kings,
 * and checks square occupation and capture conditions.
 * 
 * @param piece The piece to move.
 * @param newRow The row to move to.
 * @param newCol The column to move to.
 * @return true if the move is valid, false otherwise.
 */
bool CheckersBoard::isValidMove(Piece *piece, int newRow, int newCol)
{
    // Null check
    if (!piece) {
        return false;
    }

    // Checks if destination is occupied
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *otherPiece = dynamic_cast<Piece *>(item);
        if (otherPiece && otherPiece->getRow() == newRow && otherPiece->getCol() == newCol) {
            return false;
        }
    }

    // Gathers movement deltas
    int oldRow = piece->getRow();
    int oldCol = piece->getCol();
    int rowDiff = newRow - oldRow;
    int colDiff = abs(newCol - oldCol);

    // Prevents backward movement for non-king pieces
    if (!piece->isKing()) {
        if (piece->getColor() == PieceColor::Red && rowDiff <= 0) {
            return false;
        }
        if (piece->getColor() == PieceColor::Black && rowDiff >= 0) {
            return false;
        }
    }

    // Valid 1-step king move
    if (piece->isKing() && abs(rowDiff) == 1 && colDiff == 1) {
        return true;
    }

    // Valid 1-step move (non-capture)
    if (abs(rowDiff) == 1 && colDiff == 1) {
        for (QGraphicsItem *item : m_scene->items()) {
            Piece *otherPiece = dynamic_cast<Piece *>(item);
            if (otherPiece && otherPiece->getRow() == newRow && otherPiece->getCol() == newCol) {
                return false;
            }
        }
        return true;
    }

    // If not a normal move, check if it's a valid capture
    return isCaptureMove(piece, newRow, newCol);
}

/**
 * @brief Checks if a specific move is a valid capture.
 * 
 * Validates jump distance, direction, and presence of an opponent between source and destination.
 * 
 * @param piece The piece attempting the move.
 * @param newRow The target row.
 * @param newCol The target column.
 * @return true if the move is a legal capture, false otherwise.
 */
bool CheckersBoard::isCaptureMove(Piece *piece, int newRow, int newCol)
{
    if (!piece) return false;

    // Ensure target position is within board bounds
    if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) {
        return false;
    }

    int oldRow = piece->getRow();
    int oldCol = piece->getCol();
    int rowDiff = newRow - oldRow;
    int colDiff = newCol - oldCol;

    // Prevent backward captures for non-kings
    if (!piece->isKing()) {
        if (piece->getColor() == PieceColor::Red && rowDiff <= 0)
            return false;
        if (piece->getColor() == PieceColor::Black && rowDiff >= 0)
            return false;
    }

    // Check if move is a jump of 2 squares in both row and column
    if (abs(rowDiff) == 2 && abs(colDiff) == 2) {
        int midRow = (oldRow + newRow) / 2;
        int midCol = (oldCol + newCol) / 2;

        // Confirm midpoint is within bounds
        if (midRow < 0 || midRow >= BOARD_SIZE || midCol < 0 || midCol >= BOARD_SIZE) {
            return false;
        }

        // Check destination is unoccupied
        for (QGraphicsItem *item : m_scene->items()) {
            Piece *landingPiece = dynamic_cast<Piece *>(item);
            if (landingPiece && landingPiece->getRow() == newRow && landingPiece->getCol() == newCol)
                return false;
        }

        // Check if opponent piece exists at midpoint
        for (QGraphicsItem *item : m_scene->items()) {
            Piece *midPiece = dynamic_cast<Piece *>(item);
            if (midPiece && midPiece->getRow() == midRow && midPiece->getCol() == midCol) {
                if (midPiece->getColor() != piece->getColor())
                    return true;
            }
        }
    }

    return false;
}

/**
 * @brief Checks if a piece has any available capture moves.
 * 
 * Attempts all possible jump directions to find at least one valid capture.
 * 
 * @param piece The piece to check.
 * @return true if any capture is available, false otherwise.
 */
bool CheckersBoard::isCaptureAvailable(Piece *piece)
{
    if (!piece) return false;

    int row = piece->getRow();
    int col = piece->getCol();
    int possibleDirections[] = {-2, 2}; // Capture jump distances

    // Check all possible capture directions
    for (int dr : possibleDirections) {
        for (int dc : possibleDirections) {
            int targetRow = row + dr;
            int targetCol = col + dc;
            if (isCaptureMove(piece, targetRow, targetCol))
                return true;
        }
    }

    return false;
}

/**
 * @brief Checks if any piece of the current player has a capture move.
 * 
 * Used to enforce mandatory capture rules.
 * 
 * @return true if any capture is available for currentTurn, false otherwise.
 */
bool CheckersBoard::isAnyCaptureAvailable()
{
    // Loop through all board pieces
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece *>(item);

        // Check only current player's pieces
        if (piece && piece->getColor() == currentTurn && isCaptureAvailable(piece))
            return true;
    }

    return false;
}

/**
 * @brief Checks if the given color has won the game.
 * 
 * A player wins if their opponent has no valid moves left.
 * 
 * @param color The player to check win condition for.
 * @return true if the player has won, false otherwise.
 */
bool CheckersBoard::checkwin(PieceColor color)
{
    // Determine opponent color
    PieceColor opponent = (color == PieceColor::Black) ? PieceColor::Red : PieceColor::Black;

    // If opponent has no valid moves, current player wins
    if (!hasValidMoves(opponent)) {
        qDebug() << ((color == PieceColor::Black) ? "Black wins" : "Red Wins");
        emit gameEnded(); // Signal game end
        return true;
    }

    return false;
}


/**
 * @brief Gets all pieces of a specific color on the board.
 * 
 * @param color The color to filter by.
 * @return QList of matching pieces.
 */
QList<Piece*> CheckersBoard::getPieces(PieceColor color) {
    QList<Piece*> piecesList;

    // Loop through all items and filter pieces by color
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && piece->getColor() == color)
            piecesList.push_back(piece);
    }

    return piecesList;
}


/**
 * @brief Returns all AI-controlled pieces (assumes Red is AI).
 * 
 * @return QList of red pieces.
 */
QList<Piece*> CheckersBoard::getAIPieces() {
    return getPieces(PieceColor::Red);
}

/**
 * @brief Returns all opponent pieces (assumes Black is opponent).
 * 
 * @return QList of black pieces.
 */
QList<Piece*> CheckersBoard::getOpponentPieces() {
    return getPieces(PieceColor::Black);
}

/**
 * @brief Finds a piece by its unique ID.
 * 
 * @param id The ID of the piece.
 * @return Pointer to the piece if found, nullptr otherwise.
 */
Piece* CheckersBoard::getPieceById(int id) {
    // Loop through scene items and match piece ID
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && piece->getId() == id) {
            return piece;
        }
    }

    return nullptr;
}


/**
 * @brief Creates a deep copy of the current board.
 * 
 * Copies piece positions, states, and turn info to a new CheckersBoard instance.
 * 
 * @return Pointer to the newly cloned CheckersBoard.
 */
CheckersBoard* CheckersBoard::clone() const {
    // Create a new board instance
    CheckersBoard *newBoard = new CheckersBoard();
    newBoard->currentTurn = this->currentTurn;

    // Remove default pieces from the newly initialized scene
    QList<QGraphicsItem*> defaultItems = newBoard->m_scene->items();
    for (QGraphicsItem *item : defaultItems) {
        if (dynamic_cast<Piece*>(item)) {
            newBoard->m_scene->removeItem(item);
            delete item;
        }
    }

    // Copy actual pieces from current board to the new one
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece) {
            Piece *newPiece = new Piece(piece->getColor(), piece->isKing(), piece->getRow(), piece->getCol());
            newPiece->setId(piece->getId());
            newBoard->m_scene->addItem(newPiece);
        }
    }

    return newBoard;
}

/**
 * @brief Returns the piece located at a specific row and column.
 * 
 * @param row Board row to search.
 * @param col Board column to search.
 * @return Pointer to the matching piece, or nullptr if none found.
 */
Piece* CheckersBoard::getPieceAt(int row, int col)
{
    // Loop through all items in the scene
    for (QGraphicsItem *item : m_scene->items()) {
        Piece *piece = dynamic_cast<Piece*>(item);
        if (piece && piece->getRow() == row && piece->getCol() == col)
            return piece;
    }

    return nullptr;
}

/**
 * @brief Switches the current turn to the opposing player.
 * 
 * Emits a signal to notify about the new turn.
 */
void CheckersBoard::switchTurn() 
{ 
    // Flip the current turn
    currentTurn = (currentTurn == PieceColor::Red) ? PieceColor::Black : PieceColor::Red;

    emit turnChanged(currentTurn);  // Notify UI or game logic
}

/**
 * @brief Forces the turn to the specified color.
 * 
 * Used for manual overrides or debugging.
 * @param color The color to set as the current turn.
 */
void CheckersBoard::forceTurn(PieceColor color)
{
    currentTurn = color;
}

/**
 * @brief Initializes the board squares and their layout.
 * 
 * Alternates colors for a checkered appearance and adds them to the scene.
 */
void CheckersBoard::initializeBoard() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            qreal x = col * CELL_SIZE;
            qreal y = row * CELL_SIZE;

            // Create square at (row, col)
            BoardSquare *square = new BoardSquare(row, col, x, y, CELL_SIZE, CELL_SIZE);

            // Alternate square colors
            if ((row + col) % 2 == 0)
                square->setBrush(QColor(139, 69, 19));  // Dark wood
            else
                square->setBrush(QColor(222, 184, 135)); // Light wood

            m_scene->addItem(square);
        }
    }    
}

/**
 * @brief Initializes all red and black pieces on the board.
 * 
 * Red is placed at the top rows, black at the bottom.
 */
void CheckersBoard::initializePieces()
{
    int pieceRows = (BOARD_SIZE == 8) ? 3 : 1; // Use 3 rows on a standard 8x8 board

    // Initialize Red Pieces
    for (int row = 0; row < pieceRows; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if ((row + col) % 2 == 0) {  // Place only on dark squares
                Piece *redPiece = new Piece(PieceColor::Red, false, row, col);
                // Center the piece inside the square
                qreal x = col * CELL_SIZE + (CELL_SIZE - redPiece->pixmap().width()) / 2.0;
                qreal y = row * CELL_SIZE + (CELL_SIZE - redPiece->pixmap().height()) / 2.0;
                redPiece->setPos(x, y);
                m_scene->addItem(redPiece);
            }
        }
    }

    // Initialize Black Pieces
    for (int row = BOARD_SIZE - pieceRows; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if ((row + col) % 2 == 0) {
                Piece *blackPiece = new Piece(PieceColor::Black, false, row, col);
                qreal x = col * CELL_SIZE + (CELL_SIZE - blackPiece->pixmap().width()) / 2.0;
                qreal y = row * CELL_SIZE + (CELL_SIZE - blackPiece->pixmap().height()) / 2.0;
                blackPiece->setPos(x, y);
                m_scene->addItem(blackPiece);
            }
        }
    }
}

/**
 * @brief Undoes the most recent move from move history.
 * 
 * Restores piece position, reverts promotions, and brings back captured pieces.
 */
void CheckersBoard::undoLastMove()
{
    if (moveHistory.empty()) {
        return;
    }

    // Pop the last move from history
    MoveRecord record = moveHistory.top();
    moveHistory.pop();

    // Get the piece that was moved
    Piece* piece = getPieceById(record.pieceId);
    if (!piece) {
        return;
    }

    // Move the piece back to its old position
    piece->setBoardPosition(record.oldRow, record.oldCol);
    qreal x = record.oldCol * CELL_SIZE + (CELL_SIZE - piece->pixmap().width()) / 2.0;
    qreal y = record.oldRow * CELL_SIZE + (CELL_SIZE - piece->pixmap().height()) / 2.0;
    piece->setPos(x, y);

    // If it was promoted, revert it
    if (record.wasPromoted) {
        piece->demoteFromKing();
    }

    // If a capture happened, recreate the captured piece
    if (record.captureOccurred) {
        Piece* capturedPiece = new Piece(record.capturedColor, record.capturedWasKing, record.capturedRow, record.capturedCol);
        capturedPiece->setId(record.capturedPieceId);
        qreal cx = record.capturedCol * CELL_SIZE + (CELL_SIZE - capturedPiece->pixmap().width()) / 2.0;
        qreal cy = record.capturedRow * CELL_SIZE + (CELL_SIZE - capturedPiece->pixmap().height()) / 2.0;
        capturedPiece->setPos(cx, cy);
        m_scene->addItem(capturedPiece);
    }

    // Switch turn back and emit necessary signals
    switchTurn();
    emit moveCompleted();
    emit gameCheck();
}

/**
 * @brief Highlights all valid destination squares for the selected piece.
 * 
 * Also shows captures in dark green, and normal moves in light green.
 * 
 * @param piece Pointer to the piece to check moves for.
 */
void CheckersBoard::highlightValidMoves(Piece* piece) {
    clearHighlightedSquares(); // Clear previous highlights

    for (QGraphicsItem* item : m_scene->items()) {
        BoardSquare* square = dynamic_cast<BoardSquare*>(item);
        if (!square) continue;

        int row = square->getRow();
        int col = square->getCol();

        bool isCapture = isCaptureMove(piece, row, col);
        bool isValid = isValidMove(piece, row, col);

        // Multi-capture scenario
        if (selectedPiece == piece) {
            if (isCapture) {
                square->setBrush(QColor(0, 100, 0)); // Dark green
                m_highlightedSquares.append(square);
            } else if (isValid) {
                square->setBrush(Qt::green); // Light green
                m_highlightedSquares.append(square);
            }
        }
        // Regular move
        else if (!selectedPiece && isValid) {
            if (isCapture)
                square->setBrush(QColor(0, 100, 0));
            else
                square->setBrush(Qt::green);
            m_highlightedSquares.append(square);
        }
    }
}

/**
 * @brief Clears all square highlights from previous move suggestions.
 */
void CheckersBoard::clearHighlightedSquares() {
    // Reset each highlighted square back to default
    for (BoardSquare* square : m_highlightedSquares) {
        square->resetColor();
    }

    // Clear the list
    m_highlightedSquares.clear();
}
/**
 * @brief Sets the selected piece and highlights its possible moves.
 * 
 * @param piece The piece to mark as selected.
 */
void CheckersBoard::setSelectedPiece(Piece* piece) {
    selectedPiece = piece;
    highlightValidMoves(piece);
}

/**
 * @brief Gets the color of the current player's turn.
 * 
 * This function returns the color (Red or Black) representing whose turn it is 
 * to move on the board.
 * 
 * @return PieceColor The color of the current player.
 */
PieceColor CheckersBoard::getCurrentTurn() const {
    return currentTurn;
}

/**
 * @brief Retrieves the currently selected piece on the board.
 * 
 * This function returns a pointer to the piece that is currently selected 
 * by the player. If no piece is selected, it returns nullptr.
 * 
 * @return Piece* Pointer to the selected piece, or nullptr if none.
 */
Piece* CheckersBoard::getSelectedPiece() const {
    return selectedPiece;
}