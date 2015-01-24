// territory.c:
// Individual territory functions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "territory.h"
#include "game.h"
#include "types.h"
#include "util.h"
#include "gamedata.h"

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
void doBattle(int tA, int tD)
{
    int attackers = min(3, territories[tA].troops - 1);
    int defenders = min(2, territories[tD].troops);
    
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
    if(territories[t].owner != currentPlayer)
        return 0;
    if(!isNeighbor(source, t))
        return 0;
    return 1;
}
