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
// Scheme for increasing values of cardExchangeValue.
extern CardValueScheme cardValueScheme;

// Initializes the cards system by emptying each player's hand, creating a new
// deck of cards, and shuffling the deck.
void initCards(CardValueScheme scheme);

// Take the top card from the deck and places it in the specified player's hand.
Card drawCard(int player);

// Attempts to remove the three specified cards from the player's hand, and
// returns the number of bonus troops from the exchange. Returns 0 if the trade
// was an invalid set or the player doesn't actually have the cards specified.
int exchangeCards(int player, int card1, int card2, int card3);

#endif
