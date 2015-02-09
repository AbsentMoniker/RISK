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

