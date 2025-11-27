/**
* @file CardDeck.c
*
* @date 17.11.2025
*/


#include <stdlib.h>
#include "CardDeck.h"
#include "Card.h"
#include <stdbool.h>
#include <time.h>

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
* calling this function, and will not be set to the newly added card.
* 
* @param card The card to be inserted
* @param deck The card deck to insert the card into
* @return Enum value indicating allocation success status
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
* @param deck The deck to delete the card from
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
	CHECK_DECK_VALID(deck); // return illegalCard if top card isn't valid

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
	CHECK_DECK_VALID(deck); // return illegalCard if next card isn't valid

	deck->current = deck->current->successor;
	return ok;
}

/************************************************************
* Essential Operations
************************************************************/

Card* CardDeck_seeTop(CardDeck* deck) {
	if (deck == NULL) return NULL; // if deck is null, return null
	if (deck->head->successor == NULL) return NULL; // if deck is empty, return null

	return &(deck->head->successor->card); // return pointer towards top card of deck
}

/**
* @breif Ceate Ordered decks? funtion
*
*/
CardDeck* CardDeck_createOrdered(int numPacks) {
	CardDeck* deck = CardDeck_create();
	if (!deck) return NULL;

	deck->current = deck->head;

	for (int i = 0; i < 52 * numPacks; i++) {
		Suit suit = (i % 52) / 13; // each suit has 13 cards 
		Rank rank = i % 13; // changes after counting from 0 to 12
		/*
		if (rank == 12) {
			rank = 0;
			if(suit >= 3) {
				suit = 0;
			}
			else {
				suit += 1;
			}
		}
		*/
		Card card;
		Card_create(&card, suit, rank);
		CardDeck_insertAfter(&card, deck);
		deck->current = deck->current->successor;
	}
	return deck;
}

/************************************************************
* Complex Operations
************************************************************/


/**
*randomizes the deck
* randomly mix the elements so they are at random order 
* 
*ensure pointers are correct
* no element should bebiased
* 
* e.g A->B->C->D 
* 
* randomly generate  number
* trancverse rom head to find that node num
* insert node at that position
* 
* aka i wnt to inset a node at index 5 
* 
* i need index 4 to link to the current node]
* so i decrese pos by 1 to get prev node
* i store wht prev nod is linking to to a temp variale
* put whats before  current node in a temp variable
* then prev links to currnt node
* 
* whats links to current node linked to the target node
* 
* target node liks to prevous current nodes successor 
* 
* 1.Find the length of the linked list
* 2.Pick a random node 
* 3.Generate a random index position 
* 4.Inner loop: find prevTragteNode at a certian positioj
* 4.Rearrange pointers  to move nodes to teh correct position
* 5.repeat loop
* 
* 
*/

/**
*@brief Shuffles a deck of cards using randomization
* @details This function randomizes the order of cards in a linked list deck 
* by repeatedly selecting random positions and rearranging nodes.
* The algorithm uses srand(time(NULL) which esnures unbiased shuffling
* 
* Algorithm steps:
* 1.Calculate the length of the devl
* 2.For each card 
*	-Generate a random index
*	-Tranverse to find the node at that random position
*	-Rearrange pointers to move the current card to teh random position
* 3.Update predecessor and current pointers for the next iteration
* 
* Pointer management:
*	-Different handling when prev/predecessor is null
*	-Predecessor used to track the node before the current node
* 
* Example: A->B->C->D 
* A ranodm position is selected , predecessor, current and target node links/pointers are rearranged in order
* to insert the current node at the target node's position
* 
* @param deck Pointer to the unshuffled CardDeck structure 
* 
* 
* @note Uses srand(time(NULL)) to seed  the random number generator 
* 
* 
**/
void CardDeck_shuffle(CardDeck* deck) {
	srand(time(NULL));
	CHECK_DECK_VALID(deck);
	

	
	CardNode* temp;//temp node
	CardNode* prevTargetNode;
	CardNode* prev=NULL;
	int decklen = 0;
	while (deck->current!=NULL) {
		decklen++;//get the length of deck to geneate random indexes

	}
	
	//loops while successor isnt null
	while (deck->current!=NULL) {//handles pointers
		int randIndex = rand() % decklen;//generates a random index witin the dcek length
		int pos = randIndex;// stores random index into pos
		//pos used to find the pos of the target node
		//i loop from current node to pos 
		//tempCurr = deck->current;//used to prevent losing track of the current node when I am transversing through the list
		prevTargetNode = deck->current;//prevTragte node starts at current node





		for (int i = 0; i < pos - 1; i++) {//used to find the node before current node
			prevTargetNode = prevTargetNode->successor;



		}


		//if statements to track prev node

		if (prev==NULL) {
			//A->B->C->D 
			//head=A 
			//stored currentnode in tempCurr
			//tempCurr can cange 
			//current cannot cahnge
			temp = deck->head;//first node

			//head points to where prevNode points to
			deck->head = prevTargetNode->successor;//deck head is now c

			//current head links to where the targetnode points to
			temp->successor = prevTargetNode->successor->successor;

			//prevTaregte node points to the original head
			prevTargetNode->successor = temp;



			//targetnode points to prevTargetnode 
			deck->head->successor = prevTargetNode;



		}
		else {
			temp = deck->current;//storing the reference to the currnet node into temp
			

			//prev node links to targetNode
			prev->successor = prevTargetNode->successor;

			//targetnode links to prevTarget node
			prevTargetNode->successor->successor = prevTargetNode;

			//current node links to where tragetnode links to

			temp->successor = prevTargetNode->successor->successor;

			//prev targetnode links to current node
			prevTargetNode->successor = temp;

		}
		prev = deck->current;
		deck->current = deck->current->successor;



	



	}


}

/**
*1.neeed to find out access a spcific pat in an enum  and somehow loop through it 
* 2.sorting --> loop through the list, 
* 3. if  the element is club then increment by 1 else 
* 
*  selection sort
* 
* first compare suits lower enum =smaller
* 
* if suits are the same then compare ranks
* if i <i+! then increment else swap ranks
* 
* if suits are different then swap the lrger enum with the smaller one
* 
* if it cant find smaller then make tthe ext enum value as smller
* repeat until ordered
* 
* this is going to be awkard cuz i need to take i account the last card will point to null , all i need to do is whe the las card is being swapped the last card pointts to where the card im swapping it with is pointing, and the card im swpaping with it points to null correct
* 
*/



/**
*Buble Sort
* how i will be doing this
* 1. im comapring adjcant cards
* 2. checking if current->suit >current->link->suit
*		if true then they are swapped
* 
* 3. checking if suits arre equal
*		if true then check if current->rank>current->link->rank
*		if true then the cards are swapped
* 
* 4. handle pointerr updates carefullly and 
*		dont forget when the current->link is null
*	
* 
* 
*/

/**
* @brief Implements Bubble Sort Algoirthm to sort a deck of cards
* @details This function implements Bubble Sort Algorithm to sort a deck of cards using a Linekd List Structure.
*			The sorting is based off 2 rules:
*				Rule 1: Must sort by suit (lower enum is the smaller one)
*				Rule 2: Mut sort by rank (When suits are equal)
* 
* This algorithm uses 2 while loops
*	Outer loop: Repeatedly passes through the list until the deck is in the right order and no more swaps is needed.
*	Inner loop: Compares suits and swaps once the currentsuit is greather then the successor suit
* 
* Implementation details:
* - Uses swapped boolean flag to track if swaps occured or not
* - Uses if statements to handle when current node is the head or not
* - Rearranges pointers to point to the correct node
* - After every pass , predecessor becomes null and current node egins at the head node again
* 
* 
* @param deck Pointer to the unsorted CardDeck structure 
* 
* 
* @return void: nothing
* 
* 
* 
* 
*/
void CardDeck_sort(CardDeck* deck) {
	//bubbl sort
	//sort the deck by suit then rank
	
	//current is the card
	//like curret i linked list the current is  type node
	//in type node u can get the data or link

	//so in type cardnode u can get cardata and 

	//ii need a temp variable to store reference;

	CHECK_DECK_VALID(deck);//checks if deck is valid or not



	CardNode* prev=NULL;
	CardNode* temp;
	CardNode* temp2;
	bool swapped;

	//use swapped
	//to track the passes
	//swapped=true only when adjacnet elements are swapped
	//once the end is reached swapped=false because theres nothing more toswap
	
	
	//outer loop handles the num of passes
	// //pas until no more swaps occur
	//looping until
	swapped = true;
	while (swapped==true) {
		//inner loop handles the swapping
		
		while (deck->current->successor != NULL) {//while successor  isnt null
			swapped = false;
			//swapped only stayys false if we reached then end and theres nothing left to swap
			//i wnat to acces the suit

			//deck->current is the card itself
			//deck->current->suit is the shuit
			//deck->current->rank is the rank itself

			//if current suit is greater then successor suit then swap cards


			if (deck->current->card->suit > deck->current->successor->card->suit) {

				if (prev != NULL) {
					// A->B->C->D
					// swapping B and C
					// storing c in temp
					temp = deck->current->successor;
					//b link to d 
					deck->current->successor = deck->current->successor->successor;
					//c links to b
					temp->successor = deck->current;
					//a links to c
					prev->successor = temp;
					swapped = true;

				}
				else {
					//A->B->C->D
					// we want head to point to 2nd node 
					// and the frist node to point to the 3rd node
					//swapping A and B 
					//wand to store reference to first node
					temp = deck->head;
					temp2 = deck->head->successor->successor;//storing a refernece to c
					//head links to 2nd node/B
					deck->head = deck->head->successor;
					//first node links to where the 2nd ndde was connectd to
					//B link to A
					deck->head->successor = temp;
					// link to c
					temp->successor = temp2;
					swapped = true;





				}



				//now i want the predecessor to point to the successor node

				//how do i get the predecessor

				//i have the current node
				//now i want the predecessor to point to the current node

				//e.g if i have A->B->C->D
				//and i swap b and c, b becomes the current node

				prev = deck->current;//prev is he current node -> used in next iteration
				deck->current = deck->current->successor;//amkes successor the current node


			}
			//else if the suits are the same then check rank
			else if (deck->current->card->suit == deck->current->successor->card->suit) {

				//if the currnet card's rank is > then the next card's rank then swap cards
				if (deck->current->card->rank > deck->current->successor->card->rank) {
					if (prev != NULL) {
						// A->B->C->D
						// swapping B and C
						// storing c in temp
						temp = deck->current->successor;
						//b link to d 
						deck->current->successor = deck->current->successor->successor;
						//c links to b
						temp->successor = deck->current;
						//a links to c
						prev->successor = temp;
						swapped = true;

					}
					else {
						//A->B->C->D
						// we want head to point to 2nd node 
						// and the frist node to point to the 3rd node
						//swapping A and B 
						//wand to store reference to first node
						temp = deck->head;
						temp2 = deck->head->successor->successor;//storing a refernece to c
						//head links to 2nd node/B
						deck->head = deck->head->successor;
						//first node links to where the 2nd ndde was connectd to
						//B link to A
						deck->head->successor = temp;
						// link to c
						temp->successor = temp2;
						swapped = true;





					}
					//now i want the predecessor to point to the current node
					prev = deck->current;
					deck->current = deck->current->successor;//amkes successor the current node



				}

			}
			else {
				//now i want the predecessor to point to the current node
				
				prev = deck->current;
				deck->current = deck->current->successor;//amkes successor the current node

			}











		}
		prev = NULL;//After each pass prev needs to reset to null to start where head=current--> prev cant be before current
					//thefore it has to be null
		deck->current = deck->head;
	}




}
void CardDeck_recycleHidden(CardDeck* hidden, CardDeck* played) {

}

