#include <stdio.h>
#include <stdlib.h>
#include "CardDeck.h"
#include "game.h"
#include <time.h>


int main() {
	// seed the random number generator with the current time
	srand(time(NULL));

	/********************************
	* Game initialization
	********************************/
	
	// take user input, determining number of packs to be used for the game
	int numPacks;
	printf("Enter the number of card packs to be used in the game: ");
	scanf_s("%i", &numPacks);

	// initialize decks of the game
	CardDeck* hidden = CardDeck_createOrdered(numPacks);
	CardDeck_shuffle(hidden);
	CardDeck* played = CardDeck_create();
	CardDeck* p1 = CardDeck_create();
	CardDeck* p2 = CardDeck_create();
	
	// allocate memory for game and adjust pointers
	Game* g = malloc(sizeof(Game));
	if (g == NULL) {
		printf("Allocation for game failed.\n");
		return EXIT_FAILURE;
	}
	g->hidden = hidden;
	g->played = played;
	g->p1 = p1;
	g->p2 = p2;
	g->status = ongoing;

	// setup the game
	Game_deal(g);
	Game_sortPlayers(g);
	Game_printPlayers(g);
	printf("\n");
	
	/********************************
	* Game simulation
	********************************/

	// place the first card of the played deck from the hidden deck
	deckError err = ok;
	Card firstCard = CardDeck_useTop(g->hidden, &err);
	err = CardDeck_insertToTop(g->played, firstCard);
	if (err != ok) {
		// if we can't use the top card of the shuffled deck and insert it into the empty played deck, return error
		printf("Error: could not set the first card of the played deck.\n");
		return err;
	}
	printf("First card has been placed: ");
	Card_print(&firstCard);
	int playerTurn = 0;
	g->status = ongoing;

	while (g->status == ongoing) {
		playerTurn = (playerTurn % 2) + 1;
		Game_playTurn(g, playerTurn);

		// check if a player has an empty deck, set the status correspondingly
		if (g->p1->head->successor == NULL) {
			printf("Player 1 wins!\n");
			g->status = win;
		}
		else if (g->p2->head->successor == NULL) {
			printf("Player 2 wins!\n");
			g->status = win;
		}
	}

	/********************************
	* Game end
	********************************/

	// de-allocate everything
	CardDeck_delete(hidden);
	CardDeck_delete(played);
	CardDeck_delete(p1);
	CardDeck_delete(p2);
	free(g);
	hidden = NULL;
	played = NULL;
	p1 = NULL;
	p2 = NULL;
	g = NULL;

	return EXIT_SUCCESS;	
}

