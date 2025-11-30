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

typedef enum { // enum used to indicate current status of a game.
	ongoing,
	win
} GameStatus;

typedef struct { // struct containing all components of a game.
	CardDeck* hidden;
	CardDeck* played;
	CardDeck* p1;
	CardDeck* p2;
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