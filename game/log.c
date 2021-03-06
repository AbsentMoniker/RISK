// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.

#include "types.h"
#include "log.h"
#include "io.h"

#include <stdlib.h>

#ifdef HEAP
LogEntry * gamelog;
int gamelogCapacity;
int gamelogSize;
#else
LogEntry stLog[40000];
LogEntry * gamelog = stLog;
int gamelogCapacity = 40000;
int gamelogSize;
#endif

void addLogEntry(LogEntry entry)
{
    if(gamelogSize < gamelogCapacity)
    {
        gamelog[gamelogSize++] = entry;
        return;
    }

#ifdef HEAP
    gamelogCapacity += 2000;
    gamelog = realloc(gamelog, gamelogCapacity);
    if(!gamelog)
    {
        PANIC("failed to allocate memory for game log");
    }
    gamelog[gamelogSize++] = entry;
#else
    PANIC("game log overflowed, no heap available");
#endif
}

void clearLog()
{
    gamelogSize = 0;
}

void logAttack(int aPlayer, int dPlayer, int aTerritory, int dTerritory)
{
    LogEntry le = {
        .attack = {LOG_ATTACK, aPlayer, dPlayer, aTerritory, dTerritory}
    };
    addLogEntry(le);
}

void logBattle(int aPlayer, int dPlayer, int aD1, int aD2, int aD3, 
        int dD1, int dD2)
{
    LogEntry le = {
        .battle = {LOG_BATTLE, aPlayer, dPlayer, aD1, aD2, aD3, dD1, dD2}
    };
    addLogEntry(le);
}

void logConquer(int aPlayer, int territory, int troops)
{
    LogEntry le = {
        .conquer = {LOG_CONQUER, aPlayer, territory, troops}
    };
    addLogEntry(le);
}
void logMove(int player, int sTerritory, int dTerritory, int troops)
{
    LogEntry le = {
        .move = {LOG_MOVE, player, sTerritory, dTerritory, troops}
    };
    addLogEntry(le);
}
void logCard(int player, CardType cardtype, int territory)
{
    LogEntry le = {
        .cardgiven = {LOG_CARD_GIVEN, player, cardtype, territory}
    };
    addLogEntry(le);
}
void logExchange(int player, CardType type1, CardType type2, CardType type3, 
        int troops)
{
    LogEntry le = {
        .exchange = {LOG_CARD_EXCHANGE, player, type1, type2, type3, troops}
    };
    addLogEntry(le);
}
void logTurnEnd(int player, int nextPlayer, int roundEnd)
{
    LogEntry le = {
        .turnend = {LOG_TURN_END, player, nextPlayer, roundEnd}
    };
    addLogEntry(le);
}


static int loggingPlayer = 0;
static int loggedTroops[NUM_TERRITORIES] = {0};
void writeReinforceLogs()
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        if(loggedTroops[i] != 0)
        {
            LogEntry le = {
                .reinforce = {LOG_REINFORCE, loggingPlayer, i, loggedTroops[i]}
            };
            addLogEntry(le);
            loggedTroops[i] = 0;
        }
    }
}

void logReinforce(int player, int territory, int troops)
{
    if(territory == -1)
    {
        writeReinforceLogs();
        return;
    }

    // Each log has only 12 bits for troop number, so in the rare case where
    // some player is placing more than that, we need to write out immediately.
    if(player != loggingPlayer || loggedTroops[territory] + troops >= 0xFFF)
        writeReinforceLogs();

    loggingPlayer = player;
    loggedTroops[territory] += troops;
}


