/**
 * @file CardDeck.h
 * Provides interface for the entire card deck.
 * 
 * CardDeck will be implemented as a single linked list.
 * This linked list uses dummy nodes for both the head
 * and tail, containing no card data.
 *
 * A "CardDeck" is a collection of 0,1 or more cards.
 * This data type supports any number of packs
 * of cards.
 *
 * @date 17.11.2025
*/

#ifndef CARDDECK_H
#define CARDDECK_H

#include "Card.h"

// checks if a deck is valid, else returns illegal card. use in functions returning deckError types
#define CHECK_DECK_VALID(deck) if(deck == NULL || deck->current == NULL || deck->current->successor == NULL) return illegalCard

/**
 * @brief Error codes returned by deck manipulation functions.
 *
 * This enumeration provides specific codes indicating the @ref CardDeck
 * function statuses describing on their success or failure.
 */
typedef enum {
	/** Indicates a successful operation (no error). */
	ok,
	/** Indicates that there was an attempt operating on an invalid card. */
	illegalCard,
	/** Indicates that there was a memory allocation failure (e.g. during a card creation) */
	noMemory
} deckError;

/**
 * @brief Represents a single node in a card deck.
 *
 * This structure is based off of nodes in a linked list.
 */
typedef struct n {
	/** Stores the actual data of a the card. */
	Card card;
	/** Stores a pointer of the next node in the card deck. */
	struct n* successor;
} CardNode;

/**
* @brief Represents the entire deck of cards using a linked list.
* 
* This structure maintains the starting point and current position of the deck.
*/
typedef struct {
	/** Pointer towards the head node of a deck.
	The head node contains no actual card data, except
	the pointer towards the first node. */
	CardNode* head;
	/** Pointer towards the current node of a deck. */
	CardNode* current;
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
Card* CardDeck_seeTop(CardDeck* deck);
CardDeck* CardDeck_createOrdered(int num_packs);
deckError CardDeck_insertToTop(CardDeck* deck, Card card);
Card CardDeck_useTop(CardDeck* deck, deckError* result);

// Util Operations
CardNode* CardDeck_cardNodeAt(CardDeck* deck, int index, deckError* result);
Card CardDeck_removeAt(CardDeck* deck, int index, deckError* result);
int CardDeck_count(CardDeck* deck);
void CardDeck_print(CardDeck* deck);

// Complex Operations
void CardDeck_shuffle(CardDeck* deck);
void CardDeck_sort(CardDeck* deck);

#endif
