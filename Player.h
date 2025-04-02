/**
 * @file Player.h
 * @brief Implements the Player class used to represent a human or AI player in Checkers.
 * 
 * Provides methods for making moves, accessing player pieces, and retrieving player information.
 * Acts as a base class for player-related operations in the game.
 * 
 * @author Humzah Zahid Malik
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "checkersboard.h"
#include "piece.h"

/**
 * @class Player
 * @brief Represents a player in the Checkers game.
 * 
 * Stores the player's name and their pieces. 
 * Can make moves and acts as a base for AIPlayer.
 */
class Player {
protected:
    std::string name;           ///< Player's name
    QList<Piece*> pieces;       ///< List of player's pieces

public:
    /**
     * @brief Constructs a Player with a name and a list of pieces.
     * @param playerName Name of the player.
     * @param playerPieces Pieces controlled by the player.
     */
    Player(const std::string& playerName, QList<Piece*> playerPieces);

    /**
     * @brief Virtual destructor for safe cleanup.
     */
    virtual ~Player();

    /**
     * @brief Attempts to make a move with the given piece to the target position.
     * @param board The game board.
     * @param piece The piece to move.
     * @param toPosition The destination position as (row, col).
     * @return True if move was successful.
     */
    virtual bool makeMove(CheckersBoard& board, Piece* piece, std::pair<int, int> toPosition);

    /**
     * @brief Gets the name of the player.
     * @return Player's name.
     */
    std::string getName() const;

    /**
     * @brief Gets the list of pieces the player controls.
     * @return Const reference to list of pieces.
     */
    const QList<Piece*>& getPieces() const;
};

#endif // PLAYER_H