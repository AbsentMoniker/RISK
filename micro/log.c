// log.c:
// Game logs: creating and storing
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "types.h"
#include "log.h"

#include <stdlib.h>

LogEntry * gamelog;
int gamelogSize;
int gamelogCapacity;

void addLogEntry(LogEntry entry)
{
    if(gamelogSize < gamelogCapacity)
    {
        gamelog[gamelogSize++] = entry;
        return;
    }

    gamelogCapacity += 2000;
    gamelog = realloc(gamelog, gamelogCapacity);
    if(!gamelog)
    {
        // ...
        // help
        abort();
    }
    gamelog[gamelogSize++] = entry;
}

void clearLog()
{
    gamelogSize = 0;
}

void logBattle(int aPlayer, int dPlayer, int aD1, int aD2, int aD3, 
        int dD1, int dD2)
{
    LogEntry le = {
        .battle = {LOG_BATTLE, aPlayer, dPlayer, aD1, aD2, aD3, dD1, dD2}
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
