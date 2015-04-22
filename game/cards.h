#ifndef CARDS_H
#define CARDS_H
// cards.h:
// Functions for handling cards
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015
//

#include "types.h"
#include "limits.h"

// Current cards and number of cards held by each player.
extern Hand hands[MAX_PLAYERS];
// Number of troops for the next exchange.
extern int cardExchangeValue;
// Index into array of fixed values for increasing mode.
extern int cardValueIndex;

// Current deck and discards of cards.
extern Card deck[NUM_CARDS];
extern int deckSize;
extern Card discards[NUM_CARDS];
extern int discardsSize;

// Initializes the cards system by emptying each player's hand, creating a new
// deck of cards, and shuffling the deck.
void initCards();

// Take the top card from the deck and places it in the specified player's hand.
Card drawCard(int player);

// Attempts to remove the three specified cards from the player's hand, and
// returns the number of bonus troops from the exchange. Returns 0 if the trade
// was an invalid set or the player doesn't actually have the cards specified.
// exchangeCards expects card positions in the player's hand (0-7).
// exchangeCardIndicies expects global card indicies (0-43).
int exchangeCardsHand(int player, int card1, int card2, int card3);
int exchangeCardIndices(int player, int cardidx1, int cardidx2, int cardidx3);

// Gives all the cards in an eliminated player's hand to another player.
void takeHand(int player, int eliminatedPlayer);

#endif
