/**
 * @file game.h
 * Provides interface for the game loop:
 * Operations such as dealing cards, finding matching cards,
 * and playing a turn are declared here
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
	CardDeck hidden;
	CardDeck played;
	CardDeck p1;
	CardDeck p2;
	GameStatus status; // set as ongoing initially. when its set to win, end the game
} Game;

//function declarations
//game loop
/*
deckError Game_deal(Game* game);
int CardDeck_findMatch(Game* game);
void Game_playTurn(Game* game);
*/
#endif