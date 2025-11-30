/**
* @file game.c
* Implementation for the game loop functions
* Deals cards, finds a match and plays a turn
* Date: 27/11/2025
*/

#include <stdio.h>
#include "game.h"

/*
* Game_deal
* 
* deals the first 8 cards of the game
* always takes cards from the hidden deck and alternates betweeen player 1 and 2
* so cards 0,2,4,6 go to player 1
* cards 1,3,5,7 go to player 2
* 
* if anything goes wrong while using the deck functions it returns the error code straight away
* 
*/

deckError Game_deal(Game* game)
{
	deckError err = ok;
	// deals 16 cards total; 8 for each player
	for (int i = 0; i < 16; i++)
	{
		// take the top card from the hidden deck
		Card takenCard = CardDeck_useTop(game->hidden, &err);
		if (err != ok)
		{
			// if we cant get a card return error
			return err;
		}

		// even i -> player 1, odd i -> player 2
		if (i % 2 == 0)
		{
			// put card on top of player 1s deck
			err = CardDeck_insertToTop(game->p1, takenCard);
		}
		else
		{
			// put card on top of player 2s deck
			err = CardDeck_insertToTop(game->p2, takenCard);
		}

		if (err != ok)
		{
			// something went wrong adding the card to a players hand
			return err;
		}
	}
	return ok;
}

void Game_sortPlayers(Game* game) {
	CardDeck_sort(game->p1);
	CardDeck_sort(game->p2);
}

/*
* CardDeck_findMatch
* 
* This function looks for a card in player 1s hand
* that matches either the suit or the rank of the top card on
* the played deck
* 
* It returns:
* -the index of the first matching card
* -or -1 if no match is found
*/

int CardDeck_findMatch(Game* game)
{
	// look at the top card on the played deck
	Card* target = CardDeck_seeTop(game->played);
	if (target == NULL)
	{
		// nothing to match against
		return -1;

	}

	// start from the top of player 1s deck
	deckError err = CardDeck_gotoTop(game->p1);
	if (err != ok)

	{
		// if the deck is empty
		return -1;
	}

	int index = 0;

	// we walk through the linked list using current + gotoNextCard
	while (game->p1->current != NULL)
	{
		Card currentCard = game->p1->current->card;

		// check for same suit or rank
		if (currentCard.suit == target->suit || currentCard.rank == target->rank)
		{
			// first match found return its index
			return index;
		}
		// move to next card
		err = CardDeck_gotoNextCard(game->p1);
		if (err != ok)
		{
			break;
		}

		index++;
	}

	return -1;

}

int CardDeck_findMatchInHand(CardDeck* hand, CardDeck* playedDeck) {
	// look at the top card on the played deck
	Card* target = CardDeck_seeTop(playedDeck);
	if (target == NULL)
	{
		// nothing to match against
		return -1;

	}

	CardNode* node = hand->head->successor;
	if (node == NULL) {
		return -1; // return -1 if hand is empty
	}

	int index = 0;

	// we walk through the linked list using current + gotoNextCard
	while (node != NULL)
	{
		Card currentCard = node->card;

		// check for same suit or rank
		if (currentCard.suit == target->suit || currentCard.rank == target->rank)
		{
			// first match found return its index
			return index;
		}
		// move to next card
		node = node->successor;
		index++;
	}

	return -1;
}

void Game_recycleHidden(Game* game) {
	if (game->played->head->successor == NULL || game->played->head->successor->successor == NULL) {
		printf("Played deck is empty or only has 1 card.\n");
		return;
	}// if played deck is empty or has one card, end function

	deckError err = ok;
	Card lastCard = CardDeck_useTop(game->played, &err);
	if (err != ok || lastCard.rank == INVALID_RANK) {
		printf("Error: failed in obtaining first card of played deck.\n");
		return;
	}

	while (game->played->head->successor != NULL) {
		Card card = CardDeck_useTop(game->played, &err);
		if (err != ok || card.rank == INVALID_RANK) {
			printf("Error: failed in obtaining first card of played deck.\n");
			return;
		}
		
		err = CardDeck_insertToTop(game->hidden, card);
		if (err != ok) {
			printf("Error: failed in inserting card into hidden deck.\n");
			return;
		}
	}

	CardDeck_shuffle(game->hidden);
	err = CardDeck_insertToTop(game->played, lastCard);
}

/*
* Game_playTurn
*
* This function plays one turn of the game.
*
* Steps:
* 1. Try to find a matching card in player's hand using CardDeck_findMatchInHand
* 2. if no match is found:
* -if the hidden deck is empty recycle from played back to hidden
* -then draw one card from player's hand
* 3. If a match is found:
* -remove that card from player's hand at the given index
* -put that card into the played deck
*/
void Game_playTurn(Game* game, int player) {
	int matchIndex;

	// try and find a playable card in player's hand
	if (player == 1) {
		matchIndex = CardDeck_findMatchInHand(game->p1, game->played);
	}
	else {
		matchIndex = CardDeck_findMatchInHand(game->p2, game->played);
	}

	if (matchIndex == -1)
	{
		// no matching card so player 1 has to draw

		// check if hidden deck is empty using the head pointer
		if (game->hidden->head->successor == NULL)
		{
			// if hidden is empty recycle from played back into hidden
			Game_recycleHidden(game);
			printf("\nRecycled decks!\n\n");
			printf("Hidden "); CardDeck_print(game->hidden);
			printf("\n");
			printf("Played "); CardDeck_print(game->played);
			printf("\n");
		}

		if(game->hidden->head->successor!=NULL) //
		{ // draw one card from hidden if there is atleast one card there after recycling
			deckError err = ok;
			Card drawnCard = CardDeck_useTop(game->hidden, &err);
			if (err == ok)
			{
				if (player == 1) {
					CardDeck_insertToTop(game->p1, drawnCard);
					CardDeck_sort(game->p1);
				}
				else {
					CardDeck_insertToTop(game->p2, drawnCard);
					CardDeck_sort(game->p2);
				}
				
				printf("Player %d had no match and drew a card ", player);
				Card_print(&drawnCard);
			}
			else
			{
				printf("Error: could not draw card from hidden deck\n");

			}
		}
		else
		{
			// nothing to draw even after recycling
			printf("No cards left to draw.\n");
			return;
		}

		// turn ends here
		return;
	}
	else
	{
		// we found a matching card at matchIndex so we want to play it
		deckError err = ok;
		Card playedCard;

		if (player == 1) {
			playedCard = CardDeck_removeAt(game->p1, matchIndex, &err);
		}
		else {
			playedCard = CardDeck_removeAt(game->p2, matchIndex, &err);
		}
		
		if (err != ok)
		{
			printf("Error: could not remove matching card from player %ds hand.\n", player);
			return;
		}
		// put the removed card on top of the played deck
		err = CardDeck_insertToTop(game->played, playedCard);
		if (err != ok)
		{
			printf("Error: could not place on played deck.\n");
			return;
		}
		printf("Player %d played a matching card ", player);
		Card_print(&playedCard);
	}
}



void Game_printPlayers(Game* game) {
	printf("Player 1 ");
	CardDeck_print(game->p1);
	
	printf("Player 2 ");
	CardDeck_print(game->p2);
}








  