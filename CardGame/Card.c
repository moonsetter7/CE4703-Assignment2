/**
* @file Card.c
* Provides implementation of card data type:
* e.g. creating, printing a card
* @date 17.11.2025
*/

#include "Card.h"
#include <stdio.h>

const char* suitNames[4] = {"Club", "Spade", "Heart", "Diamond"};
const char* rankNames[13] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };

/**
* Allocates and initializes a single card structure.
* 
* @param card Pointer of the card to be modified
* @param suit Value of suit, based off of Suit enum
* @param rank Value of rank, based off of Rank enum
*/
void Card_create(Card* card, Suit suit, Rank rank) {
	card->suit = suit;
	card->rank = rank;
}

/**
* Prints the details of a card, e.g. a <i> Five of Spades>
* would be <i>Spade-Five</i>.
*
* @param card Pointer of the card to be printed
* 
*
* 
*/
void Card_print(Card* card) {
	printf("Card: %s-%s\n", suitNames[card->suit], rankNames[card->rank]);
}

