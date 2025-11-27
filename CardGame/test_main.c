#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "CardDeck.h"


int main(){
	// Creates a card deck.

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
}

