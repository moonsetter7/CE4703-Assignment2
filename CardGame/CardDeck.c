/**
* @file CardDeck.c
* Provides implementation of card deck functions:
* initializing empty/ordered decks, card deck 
* manipulation, de-allocation
*
* @date 17.11.2025
*/


#include <stdlib.h>
#include <stdio.h>
#include "CardDeck.h"
#include "Card.h"
#include <stdbool.h>


/************************************************************
* Linked List Operations
* Note that all function implementations under this operation section
* are based off of reiner's lecture 18.
************************************************************/


/**
* Creates an empty card deck in the form of a singly linked list.
* Used for player hands, and the temporary deck during shuffling.
*/
CardDeck* CardDeck_create() {
	CardDeck* deck = (CardDeck*)malloc(sizeof(CardDeck)); // create and allocate for deck
	if (deck == NULL) return NULL; // return null if memory allocation fails
	
	deck->head = (CardNode*)malloc(sizeof(CardNode)); // create and allocate for head node of deck
	if (deck->head == NULL) { // free deck and return null if memory allocation for deck head fails
		free(deck);
		return NULL;
	}

	// code for actually initializing an empty deck
	deck->head->successor = NULL; // creates tail of deck
	deck->current = deck->head; // sets current node to point towards head node
	return deck;
}

/**
* Inserts a card after the current card node of a deck.
* Note that the current node of a deck will still remain the same when
* calling this function, and will not be set to the newly added node.
*
* @param card The card to be inserted.
* @param deck The card deck to insert the card into.
* @return Enum value indicating allocation success status.
*/
deckError CardDeck_insertAfter(Card* card, CardDeck* deck) {
	if (deck->current == NULL) return illegalCard; // if current node is somehow null (maybe its a tail), return error

	CardNode* newNode = (CardNode*)malloc(sizeof(CardNode)); // create and allocate newNode
	if (newNode == NULL) return noMemory; // return noMemory if allocation fails

	// begin inserting card after current node
	newNode->card = *card; // associate card with newNode
	newNode->successor = deck->current->successor; // point newNode's successor towards next node
	deck->current->successor = newNode; // point current node's successor towards newNode

	return ok;
}

/**
* Deletes the card next to the current node of a deck.
* 
* @param deck The deck to delete the card from.
*/
deckError CardDeck_deleteNext(CardDeck* deck) {
	CHECK_DECK_VALID(deck); // return invalidCard if current card or next card is invalid
	
	CardNode* toDelete = deck->current->successor; // save pointer of next node
	deck->current->successor = toDelete->successor; // point current node's successor to the node after the node to be deleted
	free(toDelete); // deallocate deleted node from memory
	toDelete = NULL; // clear the pointer
	return ok;
}

/**
* Deletes a given deck entirely.
* 
* @param deck Card deck to be deleted.
*/
void CardDeck_delete(CardDeck* deck) {
	if (deck == NULL) return; // if deck doesn't exist, do nothing

	// set current node of deck towards head node
	// this lets us delete each card starting from the beginning
	deck->current = deck->head; 

	while (deck->head->successor != NULL) { // if the node after the head isn't the tail, delete that node
		CardDeck_deleteNext(deck);
	}

	// only the deck, head, and tail are left, so we can delete them
	if(deck->head != NULL) free(deck->head);
	free(deck);
}

/**
* Sets the current node to the top card of a deck,
* i.e. the node succeeding the head node.
*
* @param deck Card deck to be deleted.
*/
deckError CardDeck_gotoTop(CardDeck* deck) {
	CHECK_DECK_VALID(deck); // return illegalCard if top card isn't valid (i.e. when top card doesn't exist)

	// set current to top card
	deck->current = deck->head->successor;
	return ok;
}

/**
* Sets the current node to the next card of a deck.
*
* @param deck Card deck whose current node will be changed.
*/
deckError CardDeck_gotoNextCard(CardDeck* deck) {
	CHECK_DECK_VALID(deck); // return illegalCard if next card isn't valid (i.e. when next card is a tail)

	deck->current = deck->current->successor;
	return ok;
}

/************************************************************
* Essential Operations
************************************************************/

/**
* Gets the pointer of the card at the top of a
* given deck.
*
* @param deck Card deck to check the top card of.
* @return Pointer value of the deck's top card.
*/
Card* CardDeck_seeTop(CardDeck* deck) {
	if (deck == NULL) return NULL; // if deck is null, return null
	if (deck->head->successor == NULL) return NULL; // if deck is empty, return null

	return &(deck->head->successor->card); // return pointer towards top card of deck
}

/**
* Creates an ordered deck consisting of <i>numPacks</i> card packs.
* 
* @param numPacks no. of card packs to be inserted into the new deck.
* @return Pointer value of the newly created ordered deck.
*/
CardDeck* CardDeck_createOrdered(int numPacks) {
	CardDeck* deck = CardDeck_create();
	if (!deck) return NULL;

	deck->current = deck->head;

	for (int i = 0; i < 52; i++) {
		Suit suit = (i % 52) / 13; // each suit has 13 cards 
		Rank rank = i % 13; // changes after counting from 0 to 12

		Card card; // store card data on stack
		Card_create(&card, suit, rank);
		for (int i = 0; i < numPacks; i++) { // insert the card into the new deck numPacks amount of times
			if (CardDeck_insertAfter(&card, deck) != ok) {
				printf("Error: Could not insert card into deck due to allocation failure.\n");
				return deck;
			}
			deck->current = deck->current->successor;
		}
		
	}
	return deck;
}

/**
* Inserts a given card to the top of a specified deck.
*
* @param deck Pointer of the deck to insert the card in.
* @param card The card to be inserted into the deck.
* @return Enum value indicating the insertion status.
*/
deckError CardDeck_insertToTop(CardDeck* deck, Card card){
	if (!deck || !deck->head) return illegalCard;

	CardNode* newNode = (CardNode*)malloc(sizeof(CardNode)); // create and allocate newNode
	if (newNode == NULL) return noMemory; // return noMemory if allocation fails

	// begin inserting card after current node
	newNode->card = card; // associate card with newNode
	newNode->successor = deck->head->successor; // point newNode's successor towards next node
	deck->head->successor = newNode; // point current node's successor towards newNode

	return ok;
}

/**
* Removes the top card of a deck and returns its value.
* 
* @param deck Pointer of the deck to use the top card of.
* @param result deckError pointer indicating the card 
* value fetch status.
* @return Card value of the deck's top card.
*/
Card CardDeck_useTop(CardDeck* deck, deckError* result) {
	if (deck == NULL || deck->head->successor == NULL) {
		if (result) {
			*result = illegalCard;
		}
		return INVALID_CARD; // if deck is null, return null, if deck is empty, return null
	}

	CardNode* delnode = deck->head->successor;
	deck->head->successor = delnode->successor;
	Card delcard = delnode->card;
	free(delnode);
	if (result) *result = ok;
	return delcard;
}

/************************************************************
* Utility Operations
************************************************************/


/**
* Finds the pointer of a card deck's node at a specified index.
*
* @param deck Pointer of the deck to find the card node index of.
* @param index Index of the card node.
* @param result deckError pointer indicating the card node validity.
* @return Pointer of card node.
*/
CardNode* CardDeck_cardNodeAt(CardDeck* deck, int index, deckError* result) {
	if (deck == NULL || deck->head->successor == NULL) {
		if (result) {
			*result = illegalCard;
		}
		return NULL; // if deck is null/empty, return null
	}
	CardNode* node = deck->head->successor;
	int i = 0;
	while (i != index) {
		if (node->successor == NULL) {
			*result = illegalCard;
			return NULL;
		}
		node = node->successor;
		i++;
	}
	*result = ok;
	return node;
}

/**
* Removes a card from a card deck at a specified index and returns
* its card value.
*
* @param deck Pointer of the deck to remove a card from.
* @param index Index of the card node.
* @param result deckError pointer indicating the card node validity.
* @result Value of the removed card.
*/
Card CardDeck_removeAt(CardDeck* deck, int index, deckError* result) {
	// we must first update the predecessor to point past the node to be deleted
	CardNode* preNode = NULL;
	CardNode* delNode = NULL;

	if (index == 0) { // if index is at 0, set predecessor to head
		if (deck == NULL || deck->head->successor == NULL) {
			if (result) *result = illegalCard;
			return INVALID_CARD;
		}
		else {
			preNode = deck->head;
		}
	}
	else {  // else, set predecessor to the node prior to the deleted node
		preNode = CardDeck_cardNodeAt(deck, index - 1, result);
		if (preNode == NULL) {
			return INVALID_CARD;
		}
	}
	delNode = preNode->successor; // find the deleted node

	if (delNode == NULL) { // check if a node existed after preNode
		if (result) *result = illegalCard; // // index went out of bounds
		return INVALID_CARD;
	}

	// if all checks are fine, begin removal

	Card delCard = delNode->card;

	preNode->successor = delNode->successor; // set predecessor's successor pointer to the deleted node's successor
	free(delNode);
	if (result) *result = ok;
	return delCard;
}

/**
* Calculates the size of a deck,
*
* @param deck Pointer of the deck to find the size of.
* @return The size of the deck.
*/
int CardDeck_count(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) { // if deck is empty, return 0
		return 0;
	}
	CardNode* node = deck->head->successor; 
	int i = 0;
	while (node != NULL) { // increase count by 1 for every valid card. stop when node is a tail (end of list)
		node = node->successor;
		i++;
	}

	return i;
}

/**
* Prints a card deck.
* 
* @param deck Pointer of the deck to print out.
* @note Output example - "deck: Diamond-Ace, Spade-Five, Heart-Jack"
*/
void CardDeck_print(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) { // if card is empty, end function
		printf("Empty deck!\n");
		return;
	}

	CardNode* node = deck->head->successor;
	printf("deck: ");
	while (node != NULL) { // print every valid card. stop iterating when node is a tail (end of list)
		printf("%s-%s", suitNames[node->card.suit], rankNames[node->card.rank]);

		if (node->successor != NULL) { // print comma and space if card isn't last element
			printf(", ");
		}
		node = node->successor; 
	}
	printf("\n");
}

/************************************************************
* Complex Operations
************************************************************/


/**
* Shuffles a deck following the algorithm outlined in the assignment.
* <ol>
*	<li>Take a source deck.</li>
*	<li>Create a temporary, empty deck.</li>
*	<li>Get a random number r that has a value between 1 and N, where N is the no. of cards in the source deck.</li>
*	<li>Remove the r-th card from the source deck and insert it to the beginning of the temporary deck.</li>
*	<li>Decrement N by one.</li>
*	<li>Repeat steps 3 to 5 until N equals 0.<li>
* </ol>
* @param deck Card deck to be shuffled
*/
void CardDeck_shuffle(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) return; // if deck is null or empty, don't shuffle

	CardDeck* shuffledDeck = CardDeck_create(); // empty deck, to be populated with shuffled cards
	if (shuffledDeck == NULL) {
		printf("Error: failed to allocate memory for shuffle deck.\n");
		return;
	}
	deckError err = ok; 

	int deckSize = CardDeck_count(deck);

	// loop transferring a random card from source deck to shuffle deck
	for (int i = deckSize; i > 0; i--) {
		int r = rand() % i;
		Card removedCard = CardDeck_removeAt(deck, r, &err); // Value of card removed from source deck at index r
		
		if (err != ok || removedCard.rank == INVALID_RANK)
		{
			printf("Error: could not remove card at %d from deck.\n", r);
			return;
		}

		err = CardDeck_insertToTop(shuffledDeck, removedCard); // insert random card to top of shuffle deck

		if (err != ok) {
			printf("Error: could not insert card at %d to shuffle deck.\n", r);
			return;
		}
	}

	deck->head->successor = shuffledDeck->head->successor; // transfer temporary shuffled deck to source deck
	free(shuffledDeck->head); // free allocated shuffle deck head
	free(shuffledDeck); // free the allocated temporary shuffled deck
	shuffledDeck = NULL;
}

/**
* Sort a deck of cards using bubble sort.
* 
* @param deck The deck to be sorted
*/
void CardDeck_sort(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL || deck->head->successor->successor == NULL) return; // if deck is null or empty/has 1 card, end function
	int last = CardDeck_count(deck);
	bool sorted = false;

	CardNode* nodePtr = NULL;

	while (!sorted) { // if the deck isn't flagged as sorted, keep iterating
		nodePtr = deck->head->successor;
		sorted = true; // assume list is sorted true until proven otherwise
		for (int i = 0; i < last-1; i++) {
			CardNode* node2Ptr = nodePtr->successor;
			Card card1 = nodePtr->card; // copy card values onto stack
			Card card2 = node2Ptr->card;
			
			if ((card1.rank > card2.rank) || (card1.rank == card2.rank && card1.suit > card2.suit)) { // if card1 is bigger than card2, swap
				// swap the data of the cards, not the nodes themselves
				Card cardtemp = card1;
				nodePtr->card = card2;
				node2Ptr->card = card1;
				sorted = false; // indicate that the list still isn't sorted
			}
			nodePtr = nodePtr->successor;
			
		}
		last--; // adjust outer boundary so sorted cards are left alone
	}
}

