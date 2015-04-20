/* 
 * File:   save.h
 * Author: jacob
 *
 * Created on April 19, 2015, 7:17 PM
 */

#ifndef SAVE_H
#define	SAVE_H

void saveGame();
void restoreGame();
void deleteSavedGame();
int isSavedGame();

// ----------------
// Data to be saved
// ----------------

// Save invalid [1]
// TOTAL: 1 byte

// Game options and basic data
//numPlayers [1]
//randomDeploy [1]
//multipleDeploy [1]
//useNeutrals [1]
//currentPlayer [1]
//state [1]
//firstPlayer [1]
//territoriesRemaining [1]
//numTroops [2]
//deployTroopsLeft [6]
//mustTrade [1]
//needCard [1]
//cardExchangeValue [2]
//cardValueScheme [1]
//cardValueIndex [1]
// TOTAL: 22 bytes
#define SAVE_BASIC_SIZE 22

// Stats data
//mostTakenTerritory [2]
//totalDiceRolls [2*MAX_PLAYERS]
//diceRolls [12*MAX_PLAYERS]
//reinforcementCount [2*MAX_PLAYERS]
//killCount [2*MAX_PLAYERS]
//deathCount [2*MAX_PLAYERS]
//longestHeldTerritory [2*MAX_PLAYERS]
//territoryHoldTimes [1*NUM_TERRITORIES]
//territoryTakenCounts [1*NUM_TERRITORIES]
// TOTAL: (22*MAX_PLAYERS + 2*NUM_TERRITORIES + 2) bytes
// (218 for full map)
#define SAVE_STATS_SIZE (22*MAX_PLAYERS + 2*NUM_TERRITORIES + 2)
#define SAVE_STATS_OFFSET (1 + SAVE_BASIC_SIZE)

// Territory data
//troops, owners [(2+1)*NUM_TERRITORIES]
// TOTAL (3*NUM_TERRITORIES) bytes
// (126 for full map)
#define SAVE_TERRITORIES_SIZE (22*NUM_TERRITORIES)
#define SAVE_TERRITORIES_OFFSET (SAVE_STATS_OFFSET + SAVE_STATS_SIZE)

// Cards data
//deckSize [1]
//discardsSize [1]
//deck [3*(NUM_TERRITORIES + 2)]
//discards [3*(NUM_TERRITORIES + 2)]
//hands [MAX_PLAYERS*(1 + 3*30)]
// TOTAL: (91*MAX_PLAYERS + 6*NUM_TERRITORIES + 14)
// (812 for full map)
#define SAVE_CARDS_SIZE (91*MAX_PLAYERS + 6*NUM_TERRITORIES + 14)
#define SAVE_CARDS_OFFSET (SAVE_TERRITORIES_OFFSET + SAVE_TERRITORIES_SIZE)

// TOTAL SIZE: (113*MAX_PLAYERS + 11*NUM_TERRITORIES + 39) bytes
// (1179 for full map)
#define SAVE_TOTAL_SIZE (1 + SAVE_BASIC_SIZE + SAVE_TERRITORIES_SIZE + SAVE_CARDS_SIZE)

#endif	/* SAVE_H */

