#include "cards.h"
#include "gamedata.h"
#include "territory.h"
#include "io.h"

Hand hands[MAX_PLAYERS];
int cardExchangeValue;
CardValueScheme cardValueScheme;

int cardValueIndex;

Card deck[NUM_CARDS];
int deckSize;
Card discards[NUM_CARDS];
int discardsSize;

#define SWAP(a,b) do{Card tmp = a; a = b; b = tmp;}while(0)

void shuffleDeck()
{
    for(int i = 0; i < deckSize; i++)
    {
        int r = randint(i, deckSize - 1);
        SWAP(deck[i], deck[r]);
    }
}

void initCards(CardValueScheme scheme)
{
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        hands[i].cards = 0;
    }
    discardsSize = 0;
    
    for(int i = 0; i < NUM_CARDS; i++)
    {
        if(i < NUM_TERRITORIES)
        {
            deck[i].territory = i;
            deck[i].type = territories[i].cardtype;
        }
        else
        {
            deck[i].territory = -1;
            deck[i].type = WILD;
        }
    }
    deckSize = NUM_CARDS;
    shuffleDeck();

    cardValueIndex = 0;
    cardValueScheme = scheme;
    // Even if the values are increasing by 1, they start at the same first
    // value. For set value sets, this number gets ignored completely.
    cardExchangeValue = cardExchangeValues[0];
}

Card drawCard(int player)
{
    if(deckSize == 0)
    {
        for(int i = 0; i < discardsSize; i++)
            deck[i] = discards[i];
        deckSize = discardsSize;
        discardsSize = 0;
        shuffleDeck();
    }

    deckSize -= 1;
    hands[player].hand[hands[player].cards] = deck[deckSize];
    hands[player].cards += 1;
    return deck[deckSize];
}

int cardSetValue(Card c1, Card c2, Card c3)
{
    int amount[4] = {0,0,0,0};
    amount[c1.type] += 1;
    amount[c2.type] += 1;
    amount[c3.type] += 1;

    if(amount[INFANTRY] == 3 || (amount[INFANTRY] == 2 && amount[WILD] == 1))
        return 4;
    if(amount[CAVALRY] == 3 || (amount[CAVALRY] == 2 && amount[WILD] == 1))
        return 6;
    if(amount[ARTILLERY] == 3 || (amount[ARTILLERY] == 2 && amount[WILD] == 1))
        return 8;
    if((amount[INFANTRY] == 1 && amount[CAVALRY] == 1 
                && amount[ARTILLERY] == 1)
            || (amount[WILD] == 1 && amount[CAVALRY] == 1 
                && amount[ARTILLERY] == 1)
            || (amount[INFANTRY] == 1 && amount[WILD] == 1 
                && amount[ARTILLERY] == 1)
            || (amount[INFANTRY] == 1 && amount[CAVALRY] == 1 
                && amount[WILD] == 1))
    {
        return 10;
    }

    return 0;
}

int exchangeCards(int player, int card1, int card2, int card3)
{
    if(card1 >= hands[player].cards || card2 >= hands[player].cards 
            || card3 >= hands[player].cards)
    {
        return 0;
    }
    if(card1 == card2 || card1 == card3 || card2 == card3)
    {
        return 0;
    }

    // Ensure that the three cards of interest are at the end of the hand.
    // The checks above ensure that the hand has at least three cards.
    int idx1 = hands[player].cards - 1;
    int idx2 = hands[player].cards - 2;
    int idx3 = hands[player].cards - 3;
    SWAP(hands[player].hand[card1], hands[player].hand[idx1]);
    SWAP(hands[player].hand[card2], hands[player].hand[idx2]);
    SWAP(hands[player].hand[card3], hands[player].hand[idx3]);

    int value = cardSetValue(hands[player].hand[idx1],
            hands[player].hand[idx2], hands[player].hand[idx3]);
    if(value == 0)
        return 0;

    // Set is known valid at this point, so the exchange will go ahead.
    hands[player].cards -= 3;

    if(cardValueScheme == SET_VALUE)
        return value;
    if(cardValueScheme == INCREASING_ONE)
    {
        cardExchangeValue += 1;
        return cardExchangeValue - 1;
    }
    if(cardValueScheme == INCREASING)
    {
        value = cardExchangeValue;
        if(cardValueIndex < NUM_EXCHANGE_VALUES - 1)
        {
            cardValueIndex += 1;
            cardExchangeValue = cardExchangeValues[cardValueIndex];
        }
        else
        {
            cardExchangeValue += 5;
        }
        return value;
    }

    // uh oh
    return 0;
}

void takeHand(int player, int eliminatedPlayer)
{
    for(int i = 0; i < hands[eliminatedPlayer].cards; i++)
    {
        hands[player].hand[hands[player].cards + i] = 
            hands[eliminatedPlayer].hand[i];
    }
    hands[player].cards += hands[eliminatedPlayer].cards;
    hands[eliminatedPlayer].cards = 0;
}
