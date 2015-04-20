/* 
 * File:   stats.h
 * Author: jacob
 *
 * Created on April 15, 2015, 11:57 AM
 */

#ifndef STATS_H
#define	STATS_H

#include "types.h"
#include "log.h"
#include "limits.h"
#include "gamedata.h"
#include "io.h"

extern int totalDiceRolls[MAX_PLAYERS];
extern int diceRolls[MAX_PLAYERS][6];

extern int reinforcementCount[MAX_PLAYERS];
extern int killCount[MAX_PLAYERS];
extern int deathCount[MAX_PLAYERS];

extern TerritoryRecord longestHeldTerritory[MAX_PLAYERS];
extern int territoryHoldTimes[NUM_TERRITORIES];

extern TerritoryRecord mostTakenTerritory;
extern int territoryTakenCounts[NUM_TERRITORIES];

int processStats();
void processBattleLog(LogBattle log);
extern LogEntry * statsLogPtr;

#endif	/* STATS_H */
