#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "CardDeck.h"


int main(){
	/*
	// creates an ace of hearts
	Card card1;
	card1.suit = HEART;
	card1.rank = ACE;

	int selecton;



	CardDeck* d = CardDeck_create();
	
	deckError err = CardDeck_insertAfter(&card1, d);
	if (err != ok) {
		printf("insert failed\n");
		return;
	}

	err = CardDeck_gotoNextCard(d);
	if (err != ok) {
		printf("goto next card failed\n");
		return;
	}

	Card_print(&d->current->card);
	*/

	// creates an ordered deck consisting of 2 playing card packs
	//CardDeck* deck = CardDeck_createOrdered(1);

	//promting user with options

	//printf("\nEnter card pack number \n");//ask user for num of packs
	//
	//printf("\nCreate empty deck \n");
	//printf("\nCreate empty deck \n");


	CardDeck* deck = CardDeck_createOrdered(2);
	CardDeck_print(deck);

	//Card card1 = { HEART, ACE };
	//Card card2 = { DIAMOND, TEN };
	//Card card3 = { SPADE, FIVE };
	//Card card4 = { HEART,SEVEN };
	//printf("Inserting ");
	//Card_print(&card1);
	//CardDeck_insertToTop(deck, card1 );

	//printf("Inserting ");
	//Card_print(&card2);
	//CardDeck_insertToTop(deck, card2);

	//printf("Inserting ");
	//Card_print(&card3);
	//CardDeck_insertToTop(deck, card3);

	//printf("Inserting ");
	//Card_print(&card4);
	//CardDeck_insertToTop(deck, card4);

	//CardDeck_print(deck);
	//printf("Deck size: %d\n", CardDeck_count(deck));

	/*deckError e;
	int index = 1;
	Card* foundCard = CardDeck_removeAt(deck, index, &e);
	if (e == ok) {
		printf("Removed ");
		Card_print(foundCard);
		printf("at %d.\n", index);
	}*/

	CardDeck_shuffle(deck);
	
	CardDeck_print(deck);



	while (deck!=NULL) {
		Card* foundCard = NULL;
		deckError statusResult;
		foundCard = CardDeck_useTop(deck, &statusResult);
		if (statusResult == ok) {
			printf("Used ");
			Card_print(foundCard);
			free(foundCard);
		}
		else {
			if (statusResult == illegalCard) {
				printf("\nPlayer has no more cards\nPlayer has won the game\n");
				return EXIT_SUCCESS;
			}
			else if (statusResult == noMemory) {
				printf("No memory available.\n");
				return EXIT_FAILURE;
			}
		}
	}
	

	
}

