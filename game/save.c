// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#include "save.h"
#include "stats.h"
#include "game.h"
#include "types.h"
#include "gamedata.h"
#include "log.h"
#include "cards.h"

#include <stddef.h> // for NULL

unsigned char saveData[FLASH_ROW_SIZE]; // size of one PIC32 flash row, 2 kB

void saveGame()
{
    // Ensure that logs and stats are up to date so that the log can safely 
    // be discarded.
    writeReinforceLogs();
    while(processStats())
    {}

    saveData[0] = 1; // save valid

    // basic
    saveData[1] = numPlayers;
    saveData[2] = randomDeploy;
    saveData[3] = multipleDeploy;
    saveData[4] = useNeutrals;
    saveData[5] = currentPlayer;
    saveData[6] = state;
    saveData[7] = firstPlayer;
    saveData[8] = territoriesRemaining;
    saveData[9] = numTroops >> 8;
    saveData[10] = numTroops & 0xFF;
    for(int i = 0; i < MAX_PLAYERS; i++)
        saveData[11 + i] = deployTroopsLeft[i];
    saveData[17] = mustTrade;
    saveData[18] = needCard;
    saveData[19] = cardExchangeValue >> 8;
    saveData[20] = cardExchangeValue & 0xFF;
    saveData[21] = cardValueScheme;
    saveData[22] = cardValueIndex;

    // stats
    saveData[SAVE_STATS_OFFSET + 0] = mostTakenTerritory.territory;
    saveData[SAVE_STATS_OFFSET + 1] = mostTakenTerritory.count;
    for(int p = 0; p < MAX_PLAYERS; p++)
    {
        saveData[SAVE_STATS_OFFSET + 2 + 0*MAX_PLAYERS + p*2] = totalDiceRolls[p] >> 8;
        saveData[SAVE_STATS_OFFSET + 2 + 0*MAX_PLAYERS + p*2 + 1] = totalDiceRolls[p] & 0xFF;
        for(int d = 0; d < 6; d++)
        {
            saveData[SAVE_STATS_OFFSET + 2 + 2*MAX_PLAYERS + p*12 + d*2] = diceRolls[p][d] >> 8;
            saveData[SAVE_STATS_OFFSET + 2 + 2*MAX_PLAYERS + p*12 + d*2 + 1] = diceRolls[p][d] & 0xFF;
        }
        saveData[SAVE_STATS_OFFSET + 2 + 14*MAX_PLAYERS + p*2] = reinforcementCount[p] >> 8;
        saveData[SAVE_STATS_OFFSET + 2 + 14*MAX_PLAYERS + p*2 + 1] = reinforcementCount[p] & 0xFF;
        saveData[SAVE_STATS_OFFSET + 2 + 16*MAX_PLAYERS + p*2] = killCount[p] >> 8;
        saveData[SAVE_STATS_OFFSET + 2 + 16*MAX_PLAYERS + p*2 + 1] = killCount[p] & 0xFF;
        saveData[SAVE_STATS_OFFSET + 2 + 18*MAX_PLAYERS + p*2] = deathCount[p] >> 8;
        saveData[SAVE_STATS_OFFSET + 2 + 18*MAX_PLAYERS + p*2 + 1] = deathCount[p] & 0xFF;
        saveData[SAVE_STATS_OFFSET + 2 + 20*MAX_PLAYERS + p*2] = longestHeldTerritory[p].territory;
        saveData[SAVE_STATS_OFFSET + 2 + 20*MAX_PLAYERS + p*2 + 1] = longestHeldTerritory[p].count;
    }
    for(int t = 0; t < NUM_TERRITORIES; t++)
        saveData[SAVE_STATS_OFFSET + 2 + 22*MAX_PLAYERS + t] = territoryHoldTimes[t];
    for(int t = 0; t < NUM_TERRITORIES; t++)
        saveData[SAVE_STATS_OFFSET + 2 + 22*MAX_PLAYERS + 1*NUM_TERRITORIES + t] = territoryTakenCounts[t];

    // territories
    for(int t = 0; t < NUM_TERRITORIES; t++)
    {
        saveData[SAVE_TERRITORIES_OFFSET + 3*t + 0] = territories[t].troops >> 8;
        saveData[SAVE_TERRITORIES_OFFSET + 3*t + 1] = territories[t].troops & 0xFF;
        saveData[SAVE_TERRITORIES_OFFSET + 3*t + 2] = territories[t].owner;
    }

    // cards
    saveData[SAVE_CARDS_OFFSET] = deckSize;
    saveData[SAVE_CARDS_OFFSET + 1] = discardsSize;
    for(int c = 0; c < NUM_TERRITORIES + 2; c++)
    {
        saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 0] = deck[c].index;
        saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 1] = deck[c].territory;
        saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 2] = deck[c].type;
    }
    for(int c = 0; c < NUM_TERRITORIES + 2; c++)
    {
        saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 0] = discards[c].index;
        saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 1] = discards[c].territory;
        saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 2] = discards[c].type;
    }
    for(int p = 0; p < MAX_PLAYERS; p++)
    {
        saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 0] = hands[p].cards;
        for(int c = 0; c < MAX_HAND_SIZE; c++)
        {
            saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 0] = hands[p].hand[c].index;
            saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 1] = hands[p].hand[c].territory;
            saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 2] = hands[p].hand[c].type;
        }
    }
}

void restoreGame()
{
    if(!isSavedGame())
        PANIC("save data is not valid");
    
    // basic
    numPlayers = saveData[1];
    randomDeploy = saveData[2];
    multipleDeploy = saveData[3];
    useNeutrals = saveData[4];
    currentPlayer = (signed)saveData[5];
    state = saveData[6];
    firstPlayer = saveData[7];
    territoriesRemaining = saveData[8];
    numTroops = ((signed)saveData[9]) << 8;
    numTroops |= saveData[10];
    for(int i = 0; i < MAX_PLAYERS; i++)
        deployTroopsLeft[i] = saveData[11 + i];
    mustTrade = saveData[17];
    needCard = saveData[18];
    cardExchangeValue = saveData[19] << 8;
    cardExchangeValue |= saveData[20];
    cardValueScheme = saveData[21];
    cardValueIndex = saveData[22];

    // stats
    mostTakenTerritory.territory = saveData[SAVE_STATS_OFFSET + 0];
    mostTakenTerritory.count = saveData[SAVE_STATS_OFFSET + 1];
    for(int p = 0; p < MAX_PLAYERS; p++)
    {
        totalDiceRolls[p] = saveData[SAVE_STATS_OFFSET + 2 + 0*MAX_PLAYERS + p*2] << 8;
        totalDiceRolls[p] |= saveData[SAVE_STATS_OFFSET + 2 + 0*MAX_PLAYERS + p*2 + 1];
        for(int d = 0; d < 6; d++)
        {
            diceRolls[p][d] = saveData[SAVE_STATS_OFFSET + 2 + 2*MAX_PLAYERS + p*12 + d*2] << 8;
            diceRolls[p][d] |= saveData[SAVE_STATS_OFFSET + 2 + 2*MAX_PLAYERS + p*12 + d*2 + 1];
        }
        reinforcementCount[p] = saveData[SAVE_STATS_OFFSET + 2 + 14*MAX_PLAYERS + p*2] << 8;
        reinforcementCount[p] |= saveData[SAVE_STATS_OFFSET + 2 + 14*MAX_PLAYERS + p*2 + 1];
        killCount[p] = saveData[SAVE_STATS_OFFSET + 2 + 16*MAX_PLAYERS + p*2] << 8;
        killCount[p] |= saveData[SAVE_STATS_OFFSET + 2 + 16*MAX_PLAYERS + p*2 + 1];
        deathCount[p] = saveData[SAVE_STATS_OFFSET + 2 + 18*MAX_PLAYERS + p*2] << 8;
        deathCount[p] |= saveData[SAVE_STATS_OFFSET + 2 + 18*MAX_PLAYERS + p*2 + 1];
        longestHeldTerritory[p].territory = saveData[SAVE_STATS_OFFSET + 2 + 20*MAX_PLAYERS + p*2];
        longestHeldTerritory[p].count = saveData[SAVE_STATS_OFFSET + 2 + 20*MAX_PLAYERS + p*2 + 1];
    }
    for(int t = 0; t < NUM_TERRITORIES; t++)
        territoryHoldTimes[t] = saveData[SAVE_STATS_OFFSET + 2 + 22*MAX_PLAYERS + t];
    for(int t = 0; t < NUM_TERRITORIES; t++)
        territoryTakenCounts[t] = saveData[SAVE_STATS_OFFSET + 2 + 22*MAX_PLAYERS + 1*NUM_TERRITORIES + t];

    // territories
    for(int t = 0; t < NUM_TERRITORIES; t++)
    {
        territories[t].troops = saveData[SAVE_TERRITORIES_OFFSET + 3*t + 0] << 8;
        territories[t].troops |= saveData[SAVE_TERRITORIES_OFFSET + 3*t + 1];
        territories[t].owner = (signed)saveData[SAVE_TERRITORIES_OFFSET + 3*t + 2];
    }

    // cards
    deckSize = saveData[SAVE_CARDS_OFFSET];
    discardsSize = saveData[SAVE_CARDS_OFFSET + 1];
    for(int c = 0; c < NUM_TERRITORIES + 2; c++)
    {
        deck[c].index = saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 0];
        deck[c].territory = (signed)saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 1];
        deck[c].type = saveData[SAVE_CARDS_OFFSET + 2 + 3*c + 2];
    }
    for(int c = 0; c < NUM_TERRITORIES + 2; c++)
    {
        discards[c].index = saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 0];
        discards[c].territory = (signed)saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 1];
        discards[c].type = saveData[SAVE_CARDS_OFFSET + 2 + 3*(NUM_TERRITORIES + 2) + 3*c + 2];
    }
    for(int p = 0; p < MAX_PLAYERS; p++)
    {
        hands[p].cards = saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 0];
        for(int c = 0; c < MAX_HAND_SIZE; c++)
        {
            hands[p].hand[c].index = saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 0];
            hands[p].hand[c].territory = (signed)saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 1];
            hands[p].hand[c].type = saveData[SAVE_CARDS_OFFSET + 2 + 6*(NUM_TERRITORIES + 2) + 91*p + 1 + 3*c + 2];
        }
    }

    // reset various state that didn't need to be saved
    currentOption = 0;
    reinforceMenu = 0;
    confirm = 0;
    updateText();
    updateContinents();
    writeReinforceLogs();
    clearLog();
    statsLogPtr = NULL;
}

void deleteSavedGame()
{
    saveData[0] = 0; // save invalid
}

int isSavedGame()
{
    return saveData[0] == 0x01;
}
