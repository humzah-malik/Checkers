/**
 * @file Player.cpp
 * @brief Implements the Player class used to represent a human or AI player in Checkers.
 * 
 * Provides methods for making moves, accessing player pieces, and retrieving player information.
 * Acts as a base class for player-related operations in the game.
 * 
 * @author Humzah Zahid Malik
 */

#include "Player.h"
#include <iostream>

/**
 * @brief Constructs a Player with given name and piece list.
 * @param playerName The player's name.
 * @param playerPieces The list of pieces assigned to the player.
 */
Player::Player(const std::string& playerName, QList<Piece*> playerPieces)
    : name(playerName), pieces(playerPieces) {}  // Initialize member variables

/**
 * @brief Virtual destructor — useful for cleanup in subclasses.
 */
Player::~Player() {}

/**
 * @brief Moves a piece if the move is valid; also manages turn switching.
 * @param board The board where the move is being made.
 * @param piece Pointer to the piece being moved.
 * @param toPosition The target row and column.
 * @return True if move is successful; false otherwise.
 */
bool Player::makeMove(CheckersBoard& board, Piece* piece, std::pair<int, int> toPosition)
{
    // Null check for safety
    if (!piece) {
        std::cerr << "Player::makeMove: Error: Trying to move a nullptr piece!" << std::endl;
        return false;
    }

    // Check if move is legal
    if (!board.isValidMove(piece, toPosition.first, toPosition.second)) {
        std::cout << "Player::makeMove: Invalid move attempt!" << std::endl;        
        return false;
    }

    // Execute move on board
    board.handleMove(piece, toPosition.first, toPosition.second);

    // If multi-jump chain ends, switch turn
    if (board.getSelectedPiece() == nullptr) {
        board.switchTurn();
    }

    // Move succeeded
    return true;
}

/**
 * @brief Returns all pieces the player controls.
 * @return Const reference to the player's piece list.
 */
const QList<Piece*>& Player::getPieces() const {
    return pieces;  // Return internal QList<Piece*>
}

/**
 * @brief Returns the player's name.
 * @return Name of the player as a std::string.
 */
std::string Player::getName() const {
    return name;  // Return name field
}