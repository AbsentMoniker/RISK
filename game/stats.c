#include "stats.h"

#include <stddef.h>

int totalDiceRolls[MAX_PLAYERS] = {0};
int diceRolls[MAX_PLAYERS][6] = {{0}};

int reinforcementCount[MAX_PLAYERS] = {0};
int killCount[MAX_PLAYERS] = {0};
int deathCount[MAX_PLAYERS] = {0};

TerritoryRecord longestHeldTerritory[MAX_PLAYERS] =
{
    {-1, 0},    {-1, 0},
#ifndef USE_SIMPLE_MAP
    {-1, 0},    {-1, 0},
    {-1, 0},    {-1, 0},
#endif
};
int territoryHoldTimes[NUM_TERRITORIES] = {0};

TerritoryRecord mostTakenTerritory = {-1, 0};
int territoryTakenCounts[NUM_TERRITORIES] = {0};

// This function will process a single log each time through, to avoid taking
// too much processor time away from more important things.
// Returns true if a log was processed
LogEntry * statsLogPtr = NULL;
int processStats()
{
    if(!statsLogPtr)
        statsLogPtr = gamelog;

    if(statsLogPtr - gamelog == gamelogSize)
    {
        // Nothing to do
        return 0;
    }

    LogEntry log = *statsLogPtr++;

    switch(log.type)
    {
        case LOG_REINFORCE:
            reinforcementCount[log.reinforce.player] += log.reinforce.troops;
            break;

        case LOG_ATTACK:
            break;

        case LOG_BATTLE:
            processBattleLog(log.battle);
            break;

        case LOG_CONQUER:
            territoryTakenCounts[log.conquer.territory]++;
            territoryHoldTimes[log.conquer.territory] = 0;

            if(territoryTakenCounts[log.conquer.territory] > mostTakenTerritory.count)
            {
                mostTakenTerritory.territory = log.conquer.territory;
                mostTakenTerritory.count = territoryTakenCounts[log.conquer.territory];
            }
            break;

        case LOG_MOVE:
            break;

        case LOG_CARD_GIVEN:
            break;

        case LOG_CARD_EXCHANGE:
            break;

        case LOG_TURN_END:
            ;
            int player = log.turnend.player;
            for(int i = 0; i < NUM_TERRITORIES; i++)
            {
                if(territories[i].owner == player)
                {
                    territoryHoldTimes[i] += 1;
                    if(territoryHoldTimes[i] > longestHeldTerritory[player].count)
                    {
                        longestHeldTerritory[player].territory = i;
                        longestHeldTerritory[player].count = territoryHoldTimes[i];
                    }
                }
            }
            break;

        default:
            PANIC("unknown log type encountered");
            break;
    }
    return 1;
}

void processBattleLog(LogBattle log)
{
    // Battle 1
    // at least 1 die is always rolled for each side, so no need to check
    if(log.aDie1 > log.dDie1)
    {
        killCount[log.attackingPlayer] += 1;
        deathCount[log.defendingPlayer] += 1;
    }
    else
    {
        deathCount[log.attackingPlayer] += 1;
        killCount[log.defendingPlayer] += 1;
    }
    

    // Battle 2
    if(log.aDie2 > 0 && log.dDie2 > 0)
    {
        if(log.aDie2 > log.dDie2)
        {
            killCount[log.attackingPlayer] += 1;
            deathCount[log.defendingPlayer] += 1;
        }
        else
        {
            deathCount[log.attackingPlayer] += 1;
            killCount[log.defendingPlayer] += 1;
        }
    }

    totalDiceRolls[log.attackingPlayer] += 1;
    diceRolls[log.attackingPlayer][log.aDie1 - 1] += 1;
    if(log.aDie2 > 0)
    {
        totalDiceRolls[log.attackingPlayer] += 1;
        diceRolls[log.attackingPlayer][log.aDie2 - 1] += 1;
    }
    if(log.aDie3 > 0)
    {
        totalDiceRolls[log.attackingPlayer] += 1;
        diceRolls[log.attackingPlayer][log.aDie3 - 1] += 1;
    }

    totalDiceRolls[log.defendingPlayer] += 1;
    diceRolls[log.defendingPlayer][log.dDie1 - 1] += 1;
    if(log.dDie2 > 0)
    {
        totalDiceRolls[log.defendingPlayer] += 1;
        diceRolls[log.defendingPlayer][log.dDie2 - 1] += 1;
    }

}

void resetStats()
{
    statsLogPtr = NULL;
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        totalDiceRolls[i] = 0;
        reinforcementCount[i] = 0;
        killCount[i] = 0;
        deathCount[i] = 0;
        longestHeldTerritory[i].territory = -1;
        longestHeldTerritory[i].count = 0;
        for(int d = 0; d < 6; d++)
            diceRolls[i][d] = 0;
    }
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        territoryHoldTimes[i] = 0;
        territoryTakenCounts[i] = 0;
    }
    mostTakenTerritory.territory = -1;
    mostTakenTerritory.count = 0;
}
