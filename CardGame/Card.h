/**
 * @file Card.h
 * Provides interface for each card,
 * the card date type, the enums of
 * suit and rank.
 * 
 * The suits and ranks are as follows:\n
 * Suits: Club, Spade, Heart, Diamond\n
 * Ranks: Two, Three, Four, Five, Six, Seven,
 * Eight, Nine, Ten, Jack, Queen, King, Ace\n
 *
 * @date 17.11.2025
*/

#ifndef CARD_H
#define CARD_H

typedef enum {
	CLUB,
	SPADE,
	HEART,
	DIAMOND

} Suit;

typedef enum{
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE
} Rank;

typedef struct {
	Suit suit;
	Rank rank;
} Card;

extern const char* suitNames[4];
extern const char* rankNames[13];

void Card_create(Card* card, Suit suit, Rank rank);
void Card_print(Card* card);

#endif
