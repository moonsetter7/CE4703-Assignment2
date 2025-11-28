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
	int i;

	// deals 8 cards total
	for (i = 0; i < 8; i++)
	{
		// take the top card from the hidden deck
		Card takenCard = CardDeck_useTop(&game->hidden, &err);
		if (err != ok)

			// if we cant get a card return error
			return err;
	}

	// even i -> player 1, odd i -> player 2
	if (i % 2 == 0)

	{
		// put card on top of player 1s deck
		err = CardDeck_insertToTop(&game->p1, takenCard);
	}

	else

	{
		// put card on top of player 2s deck
		err = CardDeck_insertToTop(&game->p2, takenCard);
	}

	if (err != ok)
	{
		// something went wrong adding the card to a players hand
		return err;
	}

return ok;

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
	Card* target = CardDeck_seeTop(&game->played);
	if (target == NULL)
	{
		// nothing to match against
		return -1;

	}

	// start from the top of player 1s deck
	deckError err = CardDeck_gotoTop(&game->p1);
	if (err != ok)

	{
		// if the deck is empty
		return -1;
	}

	int index = 0;

	// we walk through the linked list using current + gotoNextCard
	while (game->p1.current != NULL)
	{
		Card currentCard = game->p1.current->card;

		// check for same suit or rank
		if (currentCard.suit == target->suit || currentCard.rank == target->rank)
		{
			// first match found return its index
			return index;
		}
		// move to next card
		err = CardDeck_gotoNextCard(&game->p1);
		if (err != ok)
		{
			break;
		}

		index++;
	}

	return -1;

}

/*
* Game_playTurn
* 
* This function plays one turn for player 1
* 
* Steps:
* 1. Try to find a matching card in player 1s hand using CardDeck_findMatch
* 2. if no match is found:
* -if the hidden deck is empty recycle from played back to hidden
* -then draw one card from player 1s hand 
* 3. If a match is found:
* -remove that card from player 1s hand at the given index
* -put that card into the played deck
*/

void Game_playTurn(Game* game)
{
	// try to find a playable card in player 1s hand
	int matchIndex = CardDeck_findMatch(game);

	if (matchIndex == -1)
	{
		// no matching card so player 1 has to draw

		// check if hidden deck is empty using the head pointer
		if (game->hidden.head == NULL)
		{
			// if hidden is empty recycle from played back into hidden
			CardDeck_recycleHidden(&game->hidden, &game->played);
		}

		// draw one card from hidden if there is atleast one card there
		if (game->hidden.head != NULL)
		{
			deckError err = ok;
			Card drawnCard = CardDeck_useTop(&game->hidden, &err);
			if (err == ok)
			{
				CardDeck_insertToTop(&game->p1, drawnCard);
				printf("Player 1 had no match and drew a card.\n");
			}
			else
			{
				printf("Error: could not draw card from hidden decl\n");

			}
		}
		else
		{
			// nothing to draw even after recycling
			printf("No cards left to draw.\n");
		}
	 
		// turn ends here
		return;
	}   
	else
	{
		// we found a matching card at matchIndex so we want to play it
		deckError err = ok;
		Card playedCard;
		
		playedCard = CardDeck_removeAt(&game->p1, matchIndex, &err);
		if (err != ok)
		{
			printf("Error: could not remove matching card from player 1s hand.\n");
			return;
		}
		// put the removed card on top of the played deck
		err = CardDeck_insertToTop(&game->played, playedCard);
		if (err != ok)
		{
			printf("Error: could not place on played deck.\n");
			return;
		}

		printf("Player 1 played a matching card.\n");


	}



}









  