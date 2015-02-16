#ifndef TERRITORY_H
#define TERRITORY_H
// territory.h:
// Territory structure and individual territory functions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

// Various helper functions
int isNeighbor(int territory1, int territory2);
void doBattle(int territoryA, int territoryD);

// Predicates used to cycle through territories for some purpose
int predUnowned(int territory);
int predOwnedCurrent(int territory);
int predOwnerOther(int territory);
int predAttackSource(int territory);
int predAttackTarget(int territory);
int predMoveSource(int territory);
int predMoveTarget(int territory);

// Fills a static array with all territories in a single connected group with
// sourceTerritory, for use in predMoveTarget.
void computeConnected(int sourceTerritory);

#endif
