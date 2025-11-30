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

/**
 * @brief Represents the different suits of a card.
 *
 * This enumeration provides the suits of a card.
 */
typedef enum {
	/** Club suit */
	CLUB,
	/** Spade suit */
	SPADE,
	/** Heart suit */
	HEART,
	/** Diamond suit */
	DIAMOND,
	/** Sentinel value for an invalid suit */
	INVALID_SUIT = -1

} Suit;

/**
 * @brief Represents the different ranks of a card.
 *
 * This enumeration provides the ranks of a card.
 */
typedef enum {
	/** Rank Two */
	TWO,
	/** Rank Three */
	THREE,
	/** Rank Four */
	FOUR,
	/** Rank Five */
	FIVE,
	/** Rank Six */
	SIX,
	/** Rank Seven */
	SEVEN,
	/** Rank Eight */
	EIGHT,
	/** Rank Nine */
	NINE,
	/** Rank Ten */
	TEN,
	/** Rank Jack */
	JACK,
	/** Rank Queen */
	QUEEN,
	/** Rank King */
	KING,
	/** Rank Ace */
	ACE,
	/** Sentinel value for an invalid rank */
	INVALID_RANK = -1
} Rank;

/**
 * @brief Represents a single playing card.
 *
 * A card is composed of a @ref Suit and a @ref Rank.
 */
typedef struct {
	/** The suit of the card, e.g., CLUB, HEART. */
	Suit suit;
	/** The rank of the card, e.g., ACE, KING, TWO. */
	Rank rank;
} Card;

extern const Card INVALID_CARD;
extern const char* suitNames[4];
extern const char* rankNames[13];

void Card_create(Card* card, Suit suit, Rank rank);
void Card_print(Card* card);

#endif
