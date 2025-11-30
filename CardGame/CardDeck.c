/**
* @file CardDeck.c
* Provides implementation of card deck functions:
* <ul>
*	<li>initializing empty/ordered decks</li>
*	<li>card deck manipulation</li>
*	<li>card deck de-allocation</li>
* </ul>
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
* @brief Creates an empty card deck in the form of a singly linked list.
* 
* Used for player hands, and the temporary deck during shuffling.
* 
* @return Pointer to the newly created, empty deck.
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
* @brief Inserts a card after the current card node of a deck.

* @note The current node of a deck will still remain the same when
* calling this function, and will not be set to the newly added node.
*
* @param card Pointer of the card to be inserted.
* @param deck Pointer of the card deck to insert the card into.
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
* @brief Deletes the card next to the current node of a deck.
* 
* @param deck Pointer of the deck to delete the card from.
* @return Enum value indicating deletion and deallocation success status.
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
* @brief Deletes a given deck entirely.
* 
* @param deck Pointer of the card deck to be deleted.
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
* @brief Sets the current node to the top card of a deck.
* 
* i.e. the node succeeding the head node.
*
* @param deck Pointer of card deck to modify the <i>current node</i> of.
* @return Error code indicating the success status of the current node re-assignment.
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
* @param deck Pointer of the deck whose current node will be changed.
* @return Error code indicating the success status of the current node re-assignment.
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
* @brief Gets the pointer of the card at the top of a given deck.
*
* @param deck Pointer of the deck to return the top card pointer of.
* @return Pointer of the deck's top card.
*/
Card* CardDeck_seeTop(CardDeck* deck) {
	if (deck == NULL) return NULL; // if deck is null, return null
	if (deck->head->successor == NULL) return NULL; // if deck is empty, return null

	return &(deck->head->successor->card); // return pointer towards top card of deck
}

/**
* @brief Creates an ordered deck consisting of <i>numPacks</i> card packs.
* 
* @param numPacks no. of card packs to be inserted into the new deck.
* @return Pointer of the newly created ordered deck.
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
* @brief Inserts a given card to the top of a specified deck.
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
* @brief Removes the top card of a deck and returns its value.
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
* @brief Finds the pointer of a card deck's node at a specified index.
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
* @brief Removes a card from a card deck at a specified index and returns
* its card value.
*
* @param deck Pointer of the deck to remove a card from.
* @param index Index of the card node.
* @param result deckError pointer indicating the card node validity.
* @return Card value of the removed card.
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
* @brief Calculates the size of a deck.
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
* @brief Prints a card deck.
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
*@brief Shuffles a deck of cards using randomization.
* 
* @details This function shuffles the deck by repeatedly selecting random cards fromm the original deck
* and inserting them into a temporry deck.
*
* @param deck Pointer to the unshuffled CardDeck structure
*
* @author Diana Ogualiri 24353051
*
* @note The deck must be valid and not emtpty
* @note uses rand() for random number generation
* @note uses a dummy node for the head node
*
**/
void CardDeck_shuffle(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) return; // if deck is null or empty, don't shuffle
	int decklen = CardDeck_count(deck);//getting the deck length
	CardDeck* deck2 = CardDeck_create();//crating a second emty deck
	if (deck2 == NULL) {
		printf("Error: failed to allocate memory for shuffle deck.\n");
		return;
	}
	//initializing deck2
	int pos = 0;
	Card targetCard;//Card is the card itself, Card* is a pointer to the card
	deckError err = ok;

	while (decklen != 0) {
		//	printf("\nwithin the loop\n");
		pos = rand() % decklen;//generating a random position between 0 and decklen - 1

		//printf("Loop decklen=%d, pos=%d\n",decklen, pos);


		targetCard = CardDeck_removeAt(deck, pos, &err);// removing the card at that position and getting its value
		if (err != ok || targetCard.rank == INVALID_RANK ) {
			printf("targetCard is invalid\n");
			return;
		}
		else {
			//printf("card at pos %d removed successfully\n", pos);
			err = CardDeck_insertToTop(deck2, targetCard);
			if (err != ok) {
				printf("failed to insert card into deck2\n");
				return;
			}
		}
		decklen--;//reduces length to prevent strange behaviour

	}//end of while loop deck1 is now 1ength 0
	deck->head->successor = deck2->head->successor;//used to point heads successor of deck2 to the heads successor of the original deck
	deck->current = deck->head->successor;//points to the  current nodes deck
	free(deck2->head);//frees head since head is a dummy node
	free(deck2);//then deck2 is freed
}


/**
* Sort a deck of cards using bubble sort.
*
* @param deck The deck to be sorted.
*/
void CardDeck_sort(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL || deck->head->successor->successor == NULL) return; // if deck is null or empty/has 1 card, end function
	int last = CardDeck_count(deck);
	bool sorted = false;

	CardNode* nodePtr = NULL;

	while (!sorted) { // if the deck isn't flagged as sorted, keep iterating
		nodePtr = deck->head->successor;
		sorted = true; // assume list is sorted true until proven otherwise
		for (int i = 0; i < last - 1; i++) {
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
