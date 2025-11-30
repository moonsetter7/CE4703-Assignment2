/**
 * @file game.h
 * Provides interface for the game loop:
 * Operations such as dealing cards, finding matching cards,
 * and playing a turn are declared here.
 *
 * @date 22.11.2025
*/

#ifndef GAME_H
#define GAME_H
#include "CardDeck.h"

/**
* @brief The possible statuses of a game.
*
* This enumeration is used to describe the current status of a game.
*/
typedef enum {
	/** Indicates the game is still ongoing and should keep playing. */
	ongoing,
	/** Indicates that a player has won the game and should therefore end. */
	win
} GameStatus;

/**
* @brief Represents the entire game itself: the decks used, and its current status.
*
* This structure combines all components required to track a game's state, including
* the various card piles and the current status of the game.
* @note This game structure limits the possible players to only two. This limitation will be more apparent
* as you read through the <i>Game</i> functions.
*/
typedef struct {
	/** Pointer to the hidden deck of cards, used for players to draw from. */
	CardDeck* hidden;
	/** Pointer to the deck of cards consisting of all played cards. */
	CardDeck* played;
	/** Pointer to player 1's deck of cards. */
	CardDeck* p1;
	/** Pointer to player 2's deck of cards. */
	CardDeck* p2;
	/** Current status of the game (e.g. ongoing, win) */
	GameStatus status; 
} Game;

//function declarations

//game loop
deckError Game_deal(Game* game);
void Game_sortPlayers(Game* game);
int CardDeck_findMatchInHand(CardDeck* hand, CardDeck* playedDeck);
void Game_recycleHidden(Game* game);
void Game_playTurn(Game* game, int player);
void Game_printPlayers(Game* game);

#endif