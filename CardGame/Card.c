/**
* @file Card.c
* Provides implementation of card data type:
* e.g. creating, printing a card
* @date 17.11.2025
*/

#include "Card.h"
#include <stdio.h>
#include <stdlib.h>

const char* suitNames[4] = {"Club", "Spade", "Heart", "Diamond"};
const char* rankNames[13] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
int suitCount= sizeof(suitNames) / sizeof(suitNames[0]);
int rankCount = sizeof(rankNames) / sizeof(rankNames[0]);

const Card INVALID_CARD = { INVALID_SUIT, INVALID_RANK }; // represents a non-usable card. return it whenever a "Card" type function encounters an error.

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
* 
* @brief Creating a card with suit and rank
* 
* @details This method creates a card by looping through the suits and ranks
* and places each within the card.
* 
* @param newCard Pointer 
* @return Card newCard
* 
**/
Card* Card_create2(Card* newCard) {
	//Card* newCard = malloc(sizeof(Card));
	
	for (int suit = 0; suit < suitCount; suit++) {
		for (int rank = 0; rank < rankCount; rank++) {
			
			newCard->rank = rank;
			newCard->suit = suit;
			

		}
		newCard->suit = suit;//stores the enum value not the string enum;
		
	}
	return newCard;

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

