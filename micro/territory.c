// territory.c:
// Individual territory functions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "territory.h"
#include "game.h"
#include "types.h"
#include "util.h"
#include "io.h"
#include "gamedata.h"

// Array of connected territories, used to check valid move targets.
static int connectedTerritories[NUM_TERRITORIES];

int isNeighbor(int t1, int t2)
{
    for(int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if(territories[t1].neighbors[i] == t2)
        {
            return 1;
        }
    }
    return 0;
}
#define SORT(a,b) do{if(a < b) {int tmp = a; a = b; b = tmp; }}while(0)
void doBattle(int tA, int tD)
{
    int attackers = min(3, territories[tA].troops - 1);
    int defenders = min(2, territories[tD].troops);
    attackerDice[0] = randint(1,6);     
    attackerDice[1] = attackers > 1? randint(1,6) : 0;
    attackerDice[2] = attackers > 2? randint(1,6) : 0;
    defenderDice[0] = randint(1,6);     
    defenderDice[1] = defenders > 1? randint(1,6) : 0;

    SORT(attackerDice[0], attackerDice[1]);
    SORT(attackerDice[0], attackerDice[2]);
    SORT(attackerDice[1], attackerDice[2]);
    SORT(defenderDice[0], defenderDice[1]);

    // Kill some troops
    if(attackerDice[0] > defenderDice[0])
        territories[tD].troops -= 1;
    else
        territories[tA].troops -= 1;
    if(attackers > 1 && defenders > 1)
    {
        if(attackerDice[1] > defenderDice[1])
            territories[tD].troops -= 1;
        else
            territories[tA].troops -= 1;
    }
}

int predUnowned(int t)
{
    return territories[t].owner == -1;
}
int predOwnedCurrent(int t)
{
    return territories[t].owner == currentPlayer;
}
int predOwnerOther(int t)
{
    return !predOwnedCurrent(t);
}
int predAttackSource(int t)
{
    if(territories[t].owner != currentPlayer)
        return 0;
    if(territories[t].troops == 1)
        return 0;
    for(int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if(territories[t].neighbors[i] != -1 &&
                territories[territories[t].neighbors[i]].owner !=
                currentPlayer)
        {
            return 1;
        }
    }
    return 0;
}
int predAttackTarget(int t)
{
    if(territories[t].owner == currentPlayer)
        return 0;
    if(!isNeighbor(source, t))
        return 0;
    return 1;
}
int predMoveSource(int t)
{
    if(territories[t].owner != currentPlayer)
        return 0;
    if(territories[t].troops == 1)
        return 0;
    for(int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if(territories[t].neighbors[i] != -1 &&
                territories[territories[t].neighbors[i]].owner == currentPlayer)
        {
            return 1;
        }
    }
    return 0;
}
int predMoveTarget(int t)
{
    if(t == source)
        return 0;
    if(territories[t].owner != currentPlayer)
        return 0;
    if(connectedTerritories[t] != 1)
        return 0;
    return 1;
}

// Find all connected territories by doing a breadth-first search of the
// territory graph. XXX: This uses quite a bit of stack, make sure there's
// enough space on the micro!
// In connectedTerritories: 
// -1 => visited and not connected, 0 => not visited, 1 => connected
void computeConnected(int sourceTerritory)
{
    int stack[NUM_TERRITORIES];
    int stackIdx = 0;
    int sourceOwner = territories[sourceTerritory].owner;
    
    for(int i = 0; i < NUM_TERRITORIES; i++)
        connectedTerritories[i] = 0;

    stack[stackIdx++] = sourceTerritory;
    connectedTerritories[sourceTerritory] = 1;
    
    while(stackIdx > 0)
    {
        int t = stack[--stackIdx];
        for(int i = 0; i < MAX_NEIGHBORS; i++)
        {
            if(territories[t].neighbors[i] == -1)
                break;
            int neighbor = territories[t].neighbors[i];
            if(connectedTerritories[neighbor])
                continue;
            if(territories[neighbor].owner == sourceOwner)
            {
                stack[stackIdx++] = neighbor;
                connectedTerritories[neighbor] = 1;
            }
            else
                connectedTerritories[neighbor] = -1;
        }
    }
}
