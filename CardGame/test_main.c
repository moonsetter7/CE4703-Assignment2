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
	CardDeck* deck = CardDeck_createOrdered(2);
	CardNode* node = deck->head->successor;
	int i = 1;
	while (node != NULL) {
		printf("Card %d: ", i++);
		Card_print(&node->card);
		node = node->successor;
	}
}

