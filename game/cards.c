#include "cards.h"
#include "gamedata.h"
#include "territory.h"
#include "io.h"
#include "log.h"

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

void initCards()
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
            deck[i].index = i;
        }
        else
        {
            deck[i].territory = -1;
            deck[i].type = WILD;
            deck[i].index = i;
        }
    }
    deckSize = NUM_CARDS;
    shuffleDeck();

    cardValueIndex = 0;
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
    logCard(player, deck[deckSize].type, deck[deckSize].territory);
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

int exchangeCardIndices(int player, int cardidx1, int cardidx2, int cardidx3)
{
    int idx1 = -1, idx2 = -1, idx3 = -1;
    for(int i = 0; i < hands[player].cards; i++)
    {
        if(hands[player].hand[i].index == cardidx1)
            idx1 = i;
        if(hands[player].hand[i].index == cardidx2)
            idx2 = i;
        if(hands[player].hand[i].index == cardidx3)
            idx3 = i;
    }
    if(idx1 == -1 || idx2 == -1 || idx3 == -1)
        return 0;
    return exchangeCards(player, idx1, idx2, idx3);
}

int exchangeCards(int player, int idx1, int idx2, int idx3)
{
    int handsize = hands[player].cards;
    if(idx1 == idx2 || idx1 == idx3 || idx2 == idx3)
        return 0;
    if(idx1 >= handsize || idx2 >= handsize || idx3 >= handsize)
        return 0;

    Card c1 = hands[player].hand[idx1];
    Card c2 = hands[player].hand[idx2];
    Card c3 = hands[player].hand[idx3];
    int value = cardSetValue(c1, c2, c3);
    if(value == 0)
        return 0;

    
    // Add the cards to the discard pile
    discards[discardsSize++] = c1;
    discards[discardsSize++] = c2;
    discards[discardsSize++] = c3;

    // Move selected cards to end of hand, then remove them
    SWAP(hands[player].hand[idx1], hands[player].hand[handsize - 1]);
    if(idx2 == handsize - 1)
        idx2 = idx1;
    if(idx3 == handsize - 1)
        idx3 = idx1;
    SWAP(hands[player].hand[idx2], hands[player].hand[handsize - 2]);
    if(idx3 == handsize - 2)
        idx3 = idx2;
    SWAP(hands[player].hand[idx3], hands[player].hand[handsize - 3]);

    hands[player].cards -= 3;

    // Bonuses for owning the territories being traded.
    if(c1.type != WILD && territories[c1.territory].owner == player)
    {
        territories[c1.territory].troops += 2;
        logReinforce(player, c1.territory, 2);
    }
    if(c2.type != WILD && territories[c2.territory].owner == player)
    {
        territories[c2.territory].troops += 2;
        logReinforce(player, c1.territory, 2);
    }
    if(c3.type != WILD && territories[c3.territory].owner == player)
    {
        logReinforce(player, c1.territory, 2);
        territories[c3.territory].troops += 2;
    }

    // If scheme is SET_VALUE, value is already set.
    
    if(cardValueScheme == INCREASING_ONE)
    {
        value = cardExchangeValue;
        cardExchangeValue += 1;
    }
    else if(cardValueScheme == INCREASING)
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
    }

    logExchange(player, c1.type, c2.type, c3.type, value);

    return value;
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
