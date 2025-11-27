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
	CardDeck* deck = CardDeck_create();
	CardNode* node = deck->head->successor;
	int i = 1;
	while (node != NULL) {
		printf("%d: ", i++);
		Card_print(&node->card);
		node = node->successor;
	}

	Card card1 = { HEART, ACE };
	Card card2 = { DIAMOND, TEN };
	Card card3 = { SPADE, FIVE };
	printf("Inserting ");
	Card_print(&card1);
	CardDeck_insertToTop(deck, card1 );

	printf("Inserting ");
	Card_print(&card2);
	CardDeck_insertToTop(deck, card2);

	printf("Inserting ");
	Card_print(&card3);
	CardDeck_insertToTop(deck, card3);

	CardDeck_print(deck);
	printf("Deck size: %d\n", CardDeck_count(deck));

	deckError e;
	int index = 1;
	Card* foundCard = CardDeck_removeAt(deck, index, &e);
	if (e == ok) {
		printf("Removed ");
		Card_print(foundCard);
		printf("at %d.\n", index);
	}
	


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

