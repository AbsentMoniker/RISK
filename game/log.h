// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#ifndef LOG_H
#define LOG_H

#include "types.h"

extern LogEntry * gamelog;
extern int gamelogSize;

// Logs a reinforcement. Individual reinforcements are saved up so that the
// logs save space, with multiple troops in each log. If territory is -1,
// forces logs to be written immediately.
void logReinforce(int player, int territory, int troops);
// Log an attack, battle, conquer, move, card draw, exchange, or turn end.
void logAttack(int aPlayer, int dPlayer, int aTerritory, int dTerritory);
void logBattle(int aPlayer, int dPlayer, int aD1, int aD2, int aD3, 
        int dD1, int dD2);
void logConquer(int aPlayer, int territory, int troops);
void logMove(int player, int sTerritory, int dTerritory, int troops);
void logCard(int player, CardType cardtype, int territory);
void logExchange(int player, CardType type1, CardType type2, CardType type3, 
        int troops);
void logTurnEnd(int player, int nextPlayer, int roundEnd);

// Forces cached reinforce logs to be written immediately.
void writeReinforceLogs();

// Empties the entire log.
void clearLog();

#endif
