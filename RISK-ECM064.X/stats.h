/* 
 * File:   stats.h
 * Author: jacob
 *
 * Created on April 15, 2015, 11:57 AM
 */

#ifndef STATS_H
#define	STATS_H

#include "gamelogic.h"

extern int totalDiceRolls[MAX_PLAYERS];
extern int diceRolls[MAX_PLAYERS][6];

extern int reinforcementCount[MAX_PLAYERS];
extern int killCount[MAX_PLAYERS];
extern int deathCount[MAX_PLAYERS];

typedef struct {
    int territory;
    int count;
} TerritoryRecord;

extern TerritoryRecord longestHeldTerritory[MAX_PLAYERS];

extern TerritoryRecord mostTakenTerritory;

void processStats();
void processBattleLog(LogBattle log);

#endif	/* STATS_H */

