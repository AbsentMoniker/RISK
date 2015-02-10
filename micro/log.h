#ifndef LOG_H
#define LOG_H
// log.h:
// Functions for creating, storing game logs.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "types.h"

extern LogEntry * gamelog;
extern int gamelogSize;

// Logs a single battle, including dice rolls.
void logBattle(int aPlayer, int dPlayer, int aD1, int aD2, int aD3, 
        int dD1, int dD2);
// Logs a reinforcement. Individual reinforcements are saved up so that the
// logs save space, with multiple troops in each log. If territory is -1,
// forces logs to be written immediately.
void logReinforce(int player, int territory, int troops);
// Empties the entire log.
void clearLog();

#endif
