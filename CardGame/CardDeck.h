/**
 * @file CardDeck.h
 * Provides interface for the entire card deck:
 * the card deck data type, its operations such as
 * shuffling, sorting, adding or removing a card.
 * CardDeck will be implemented as a single linked list.
 * 
 * A "CardDeck" is a collection of 0,1 or more cards.
 * This data type should support any number of packs
 * of cards, where the user can indicate how many 
 * complete packs of cards are to be placed in the 
 * "CardDeck" at creation.
 *
 * @date 17.11.2025
*/

#ifndef CARDDECK_H
#define CARDDECK_H

#include "Card.h"

// checks if a deck is valid, else returns illegal card
#define CHECK_DECK_VALID(deck) if(deck == NULL || deck->current == NULL || deck->current->successor == NULL) return illegalCard

typedef enum {
	ok, // no error
	illegalCard, // card is invalid
	noMemory // no memory available for allocation
} deckError;

typedef struct n {
	Card card; // stores card data in carddeck node
	struct n* successor; // contains pointer towards next carddeck node
} CardNode;

typedef struct {
	CardNode* head; // pointer towards head of carddeck
	CardNode* current; // pointer towards current node of carddeck
} CardDeck;

// Function declarations
// When you're finished implementing a function, mark it as completed on the spreadsheet

//Linked list operations
CardDeck* CardDeck_create();
deckError CardDeck_insertAfter(Card* card, CardDeck* deck);
deckError CardDeck_deleteNext(CardDeck* deck);
void CardDeck_delete(CardDeck* deck);
deckError CardDeck_gotoTop(CardDeck* deck);
deckError CardDeck_gotoNextCard(CardDeck* deck);





// Essential operations
CardDeck* CardDeck_createOrdered(int num_packs);
deckError CardDeck_insertToTop(CardDeck* deck, Card card);
Card CardDeck_useTop(CardDeck* deck, deckError* result);

/*

// Util operations
Card CardDeck_removeAt(CardDeck* deck, int index, deckError* result);
int CardDeck_count(CardDeck* deck);
void CardDeck_print(CardDeck* deck);
*/
// Complex Operations
void CardDeck_shuffle(CardDeck* deck);
void CardDeck_sort(CardDeck* deck);
void CardDeck_recycleHidden(CardDeck* hidden, CardDeck* played);







#endif
