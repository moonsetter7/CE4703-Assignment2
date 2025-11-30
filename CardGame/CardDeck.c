/**
* @file CardDeck.c
*
* @date 17.11.2025
*/


#include <stdlib.h>
#include <stdio.h>
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
* @brief Fills the deck with cards
* @details This method takes in the pack number prompted by the user.
*  
* Formula: numPacks=numPacks*52 
* Each pack is 52 cards
* The number of cards in the deck,depends on the packet number
* 
* @param deck Pointer to the CardDeck
* @param numPacks Number of card packs to add to the deck
* @author Diana Ogualiri 24353051
* @return Pointer to the filled CardDeck
* 
* 
* 
* 
**/
CardDeck* CardDeck_fillDeck(CardDeck* deck,int numPacks) {
	//creating a deck with a number the user chooses

	
	
	Card* newCard=malloc(sizeof(Card));

	if(newCard==NULL){
		return NULL; //return null if memory allocation fails
	}	
	//looping through elements in pack
	//adding cards with suits and ranks 
	//then putting each card into the deck

	//Each card is created and inserted after the current node using insertAfter function
	for (int i = 0; i < numPacks; i++) {
		
		 for (int suit = 0; suit < suitCount; suit++) {
			 for (int rank = 0; rank < rankCount; rank++) {

				 newCard->rank = rank;//inserts rank into the card
				 newCard->suit = suit;//inserts suit into the card
				
				 CardDeck_insertAfter(newCard, deck);//each card is inserted after the current node
				 CardDeck_gotoNextCard(deck);//goes to next card after insertion


			 }
			

		 }//each card is filled 
		
	}
	return deck;// deck returned
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

deckError CardDeck_insertToTop(CardDeck* deck, Card card){
	
	CHECK_DECK_VALID2(deck);//if its null
	CardNode* newNode = (CardNode*)malloc(sizeof(CardNode)); // create and allocate newNode

	if (newNode == NULL) return noMemory; // return noMemory if allocation fails

	// begin inserting card after current node
	newNode->card = card; // associate card with newNode
	newNode->successor = deck->head->successor; // point newNode's successor towards next node
	deck->head->successor = newNode; // point current node's successor towards newNode


	return ok;
}
Card* CardDeck_useTop(CardDeck* deck, deckError* result) {
	if (deck == NULL || deck->head->successor == NULL) {
		if (result) {
			*result = illegalCard;
		}
		return NULL; // if deck is null, return null, if deck is empty, return null
	}
	CardNode* delnode = deck->head->successor;
	deck->head->successor = delnode->successor;
	Card* delcard = malloc(sizeof(Card));
	if (!delcard) {
		if (result) *result = noMemory;
		return NULL;
	}
	*delcard = delnode->card;
	free(delnode);
	if (result) *result = ok;
	return delcard;
}

/************************************************************
* Utility Operations
************************************************************/
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
	return node;
}

Card* CardDeck_removeAt(CardDeck* deck, int index, deckError* result) {
	// we must first update the predecessor to point past the node to be deleted
	CardNode* preNode = NULL;
	CardNode* delNode = NULL;
	
	if (index == 0) { // if index is at 0, set predecessor to head
		if (deck == NULL || deck->head->successor == NULL) {
			if (result) *result = illegalCard;
			return NULL;
		}
		else {
			preNode = deck->head;
		}
	}
	else {  // else, set predecessor to the node prior to the deleted node
		preNode = CardDeck_cardNodeAt(deck, index-1, result);
		if (preNode == NULL) {
			return NULL;
		}
	}
	delNode = preNode->successor; // find the deleted node

	if (delNode == NULL) { // check if a node existed after preNode
		if (result) *result = illegalCard; // // index went out of bounds
		return NULL;
	}

	// if all checks are fine, begin removal

	Card* delCard = malloc(sizeof(Card)); // allocate memory for card
	if (!delCard) {
		if (result) *result = noMemory;
		return NULL;
	}
	
	preNode->successor = delNode->successor; // set predecessor's successor pointer to the deleted node's successor

	
	*delCard = delNode->card; // copy card data from deleted node into newly allocated card
	free(delNode);
	if (result) *result = ok;
	return delCard;
}

/**
* @brief Removes a card at a specific position in the deck
* @details This function removes a card at a specific position in the deck and frees its memory.
* The deck uses a dummy head node,so position 1 refers to the first non dummy card.
*	
* 
* Algorithm:
* 1. Validates the deck and position
* 2.Transverses to find the node before the target node
* 3.Unlinks teh target node from list
* 4. Frees the target node from memory
* 
* @param Pointer to the deck 
* @param pos Position of the card to remove
* 
* @return ok if card was removed successfully
* @return illegalCard if:
*	deck is invalid,position is out of bounds,transversal fails
* 
* @note pos=1 is the first card
* @note dummy node is pos=0
* 
* @see CrdDeck_count() 
* @see getCardNodeAt() -retrieves node at a certain position
* @see CHECK_DECK_VALID
*
* @return ok 
* 
**/
deckError removeCardAt(CardDeck* deck,int pos) {
	//pos=1 =the node after dummy
	//printf("within removeCardAt function pos=%d\n", pos);
	
	CardNode* targetNode;
	CardNode* prevTargetNode;
	//int count = 0;
	CHECK_DECK_VALID2(deck);

	prevTargetNode = deck->head;//previous target node starts at head
	if (prevTargetNode==NULL) {
		printf("prevTargetNode is  null\n");
		return illegalCard;

	}
	int decklen = CardDeck_count(deck);//getting the deck length
	//printf("in rmeove card");
	//printf("within removeCardAt decklen=%d\n", decklen);
	//handles case when pos is out of bounds
	if(pos<1 || pos>decklen) {// if pos is out of bounds then return illegalCard
		//invalid position
		//printf("position out of bounds\n");
		return illegalCard;
	}
	//handles case when pos is the head node
	
		//else user wants to remove any other node
		//looping until the node previous targetnode is found
	
		
	
		for (int i = 0; i < pos - 1; i++) {
			if (prevTargetNode->successor == NULL) {
				printf("no successor found\n");
				return illegalCard;//if theres no successor then return illegal card

			}
			prevTargetNode = prevTargetNode->successor;//tranversing to find previous target node

		}
		targetNode = prevTargetNode->successor;//target node is the node to be removed
		if (targetNode== NULL) {
			printf("target node is null\n");
			return noMemory;//if theres no successor then return illegal card

		}

		prevTargetNode->successor = targetNode->successor;//previous target node links to the node after target node
		free(targetNode);//freeing the target node
		//printf("node at pos %d removed\n", pos);
		return ok;
	
	





}
/**
* 
* @brief Gets the card node at a specific position in the deck
* @details This function retrieves the card node at a specific position in the deck.
* It handles 2 cases:
* 1.When the position is out of bounds
* 2. When the position is not 1 (non-head node)
* 
* @param deck Pointer to the CardDeck
* @param pos Position of the card node to retrieve (1-based index)
* 
* @return Pointer to the CardNode at the specified position, or NULL if position is out of bounds
*   
* 
**/
CardNode* getCardNodeAt(CardDeck* deck, int pos) {

	
	//pos=1 is the succesor of head
	
	CardNode* prevTargetNode;
	CardNode* targetNode;
	if (deck==NULL) {
		return NULL;

	}
	int decklen = CardDeck_count(deck);//getting the deck length

	//handles case when pos it out of bounds
	if (pos<1 ||pos>decklen) {
		return NULL;// if pos is out of bounds then return null

	}
	//handles case when pos is 1 , where 1 is the head node


		prevTargetNode = deck->head;//previous target node starts at head	
		//loops to find node before targetnode
		for (int i = 0; i < pos - 1; i++) {
			if (prevTargetNode->successor==NULL) {
				return NULL;//if theres no successor then return null

			}
			prevTargetNode = prevTargetNode->successor;//tranversing to find previous target node

		}
		targetNode = prevTargetNode->successor;//target node is the node to be removed
		if (targetNode==NULL) {
			printf("targetnode is null");
		}
		
		//once tragetnode is found , its returned
		return targetNode;


	


}

int CardDeck_count(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) { // if card is empty, return 0
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
void CardDeck_print(CardDeck* deck) {
	if (deck == NULL || deck->head->successor == NULL) { // if card is empty, end function
		printf("Empty deck!\n");
		return;
	}
	
	CardNode* node = deck->head->successor;
	printf("deck: \n");
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


/****
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

**/
/**
*@brief Shuffles a deck of cards using randomization
* @details This function shuffles the deck by repeatedly selecting random cards fromm the original deck
* and inserting them into a temporry deck.
* 
* 
* @param deck Pointer to the unshuffled CardDeck structure 
* 
* @author Diana Ogualiri 24353051
* @return ok if shuffle succeeds, illegalCard if card retrieval/removal fails,
*			noMemory if insertion fails
* 
* @note The deck must be valid and not emtpty
* @note uses rand() for random number generation
* @note uses a dummy node for the head node
* 
* @see CardDeck_create()
* @see getCardNodeAt()
* @see CardDeck_insertToTop()
*
* 
* 
* 
**/



deckError CardDeck_shuffle(CardDeck* deck) {
	CHECK_DECK_VALID(deck);
	
	
	
	int decklen = CardDeck_count(deck);//getting the deck length
	CardDeck* deck2 = CardDeck_create();//crating a second emty deck

	

	//initializing deck2
	
	int pos = 0;
	
	CardNode* targetNode;
	
	Card targetCard;//Card is the card itself, Card* is a pointer to the card


	while (decklen!=0) {
	//	printf("\nwithin the loop\n");

		

	pos = rand() % decklen+1;//generating a random position between 1 and decklen

	//printf("Loop decklen=%d, pos=%d\n",decklen, pos);

	
	targetNode = getCardNodeAt(deck, pos);//getting the card node at that position
	if (targetNode==NULL) {
		//printf("targetNode is null\n");
		return illegalCard;
	}
	//printf("targetNode found: rank=%d, suit=%d\n", targetNode->card.rank, targetNode->card.suit);
	targetCard = targetNode->card;//storing cardNodes card into targetCard
	//*targetCard = &(targetNode->card);
	deckError removeCardError = removeCardAt(deck, pos);//removing the card at that position

	if (removeCardError!=ok) {
		printf("failed to remove card at pos %d\n", pos);
		return illegalCard;

	}
	else {
		//printf("card at pos %d removed successfully\n", pos);
		deckError insertCardError=CardDeck_insertToTop(deck2, targetCard);
		if (insertCardError!=ok) {
			printf("failed to insert card into deck2\n");
			return noMemory;
		}
		

	}
	
	
	decklen--;//reduces length to prevent strange behaviour

	}//end of while loop deck1 is now 1ength 0

	

	printf ("exiting loop\n");

	

	

	
	deck->head->successor= deck2->head->successor;//used to point heads successor of deck2 to the heads successor of the original deck
	deck->current = deck2->current;//points to the  current nodes deck
	
	
	free(deck2->head);//frees head since head is a dummy node
	free(deck2);//then deck2 is freed
	return ok;






	 
	



	

	
	
	

}

/*
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
****/



/*
*Bubble Sort
* how i will be doing this
* 1. im comapring adjcant cards
* 2. checking if current->suit >current->link->suit
*		if true then they are swapped
* 
* 3. checking if suits arre equal
*		if true then check if current->rank>current->link->rank
*		if true then the cards are swapped
* 
* 4. handle pointer updates carefullly and 
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
* @author Diana Ogualiri 24353051
* 
* @return void: nothing
* 
* 
* 
* 
*/
void CardDeck_sort(CardDeck* deck) {
	//bubble sort
	//sort the deck by suit then rank
	
	//current is the card
	//like curret i linked list the current is  type node
	//in type node u can get the data or link

	//so in type cardnode u can get cardata and 

	//ii need a temp variable to store reference;

	//CHECK_DECK_VALID(deck);//checks if deck is valid or not



	CardNode* prev=NULL;//at teh start
	CardNode* temp;
	CardNode* temp2;
	bool swapped;
	
	//use swapped
	//to track the passes
	//swapped=true only when adjacnet elements are swapped
	//once the end is reached swapped=false because theres nothing more toswap
	
	
	//outer loop handles the num of passes
	// //pass until no more swaps occur

	swapped = true;
	deck->current = deck->head->successor;
	while (swapped==true) {
		//inner loop handles the swapping

		
		while (deck->current->successor != NULL) {//while successor  isnt null
			swapped = false;
			//swapped only stayys false if we reached the end and theres nothing left to swap
			//i wantt to acces the suit

			//deck->current is the card itself
			//deck->current->suit is the shuit
			//deck->current->rank is the rank itself

			//if current suit is greater then successor suit then swap cards


			if (deck->current->card.suit > deck->current->successor->card.suit) {

				if (prev==NULL) {//prev is null only when currnet node is heads successor
				//	temp2 = deck->current;
					temp = deck->head->successor;
					temp2 = deck->head->successor->successor;

					deck->head->successor = temp2;
					temp->successor = temp2->successor;
					temp2->successor = temp;
					swapped = true;




				}
				else {
					//swappng when prev isnt null
					temp = deck->current;
					temp2 = deck->current->successor;
					prev->successor = deck->current->successor;//current is d now
					temp->successor = temp2->successor;
					temp2->successor = temp;
					swapped = true;

				}

				prev = deck->current;//prev is he current node -> used in next iteration
				deck->current = deck->current->successor;//amkes successor the current node


			}
			//else if the suits are the same then check rank
			else if (deck->current->card.suit == deck->current->successor->card.suit) {

				//if the currnet card's rank is > then the next card's rank then swap cards
				if (deck->current->card.rank > deck->current->successor->card.rank) {
					temp = deck->current;

					if (prev == NULL) {//prev is null only when currnet node is heads successor
						//	temp2 = deck->current;
						temp = deck->head->successor;
						temp2 = deck->head->successor->successor;

						deck->head->successor = temp2;
						temp->successor = temp2->successor;
						temp2->successor = temp;
						swapped = true;




					}
					else {
						//swappng when prev isnt null
						temp = deck->current;
						temp2 = deck->current->successor;
						prev->successor = deck->current->successor;//current is d now
						temp->successor = temp2->successor;
						temp2->successor = temp;
						swapped = true;

					}
					
					//now i want the predecessor to point to the current node
					prev = temp2;
					deck->current = temp;//amkes successor the current node



				}
				else {
					//when ranks are in the right order then
					prev = deck->current;
					deck ->current = deck->current->successor;
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

/**
*
* @brief Puts cards from played deck to hidden deck when played deck is empty(top card left)
* @details This function transfers all cards(except the top card) from played deck to the hidden deck.
* Once the played deck only has the top card, the hidden deck is then shuffled.
* Using a temp node to store a reference to the 2nd node in the played deck, to prevent losing access to it.
* 
* @param hidden Pointer to hidden deck(piace to draw cards from)
* @param played Pointer to played deck
* 
* @note if the played deck is empty then the fucntion returns
* 
* @author Diana Ogualiri 24353051
* @see cardDeck_shuffle()
* 
* 
**/
deckError CardDeck_recycleHidden(CardDeck* hidden, CardDeck* played) {

	//tarnsfers played cards o teh hidden deck and shuffles it

	CHECK_DECK_VALID(hidden);//checkin the hidden deck
	CHECK_DECK_VALID(played);//checking the played deck
	CardNode* topCard=played->head;//gets the top card from the played deck and stores a reference to the head
	CardNode* currentCard;//the 2nd card onwards in the played deck
	CardNode* temp;//temporary holds a card
	
	if (topCard==NULL) {
		//if topCard aka the head is null that means the deck is null
		return illegalCard;

	}

	//i want to move every card but the top card to hidden deck
	

	//so i want to start at the 2nd card so where the head points to
	currentCard = played->head->successor;//currentCard is the 2nd card

	//getting each card from played deck so I need to loop

	while (currentCard!=NULL) {//while the currnet card isnt null loop

		//looping through played deck I want to move each card to the hidden deck

		
		// the currentcard to link to the head 
		//then make the current code the headnode
		//swapping occurs
		temp = currentCard->successor;//storing a reference to where the currentacrd points to

		currentCard->successor = hidden->head;//Currentcard  links to head
		hidden->head = currentCard;

		currentCard = temp;//currentCard has the originla current card
		


	}
	//we only nat played deck to have the top card
	//we wnat the top card aka head's successor to point to nothing
	played->head->successor = NULL;
	CardDeck_shuffle(hidden);//shuffling the hiddn deck after played deck only has the top card
	return ok;




	








}

