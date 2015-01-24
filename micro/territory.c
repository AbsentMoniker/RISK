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
#define SORT(a,b) do{if(a < b) {int tmp = a; a = b; b = tmp; }}while(0)
void doBattle(int tA, int tD)
{
    int attackers = min(3, territories[tA].troops - 1);
    int defenders = min(2, territories[tD].troops);
    attackerDice[0] = randint(1,6);     
    attackerDice[1] = randint(1,6);     
    attackerDice[2] = randint(1,6);     
    defenderDice[0] = randint(1,6);     
    defenderDice[1] = randint(1,6);     

    // sort as many dice as we need
    if(attackers > 1)
    {
        SORT(attackerDice[0], attackerDice[1]);
        if(attackers > 2)
        {
            SORT(attackerDice[0], attackerDice[2]);
            SORT(attackerDice[1], attackerDice[2]);
        }
    }
    if(defenders > 1)
    {
        SORT(defenderDice[0], defenderDice[1]);
    }

    // Kill some troops
    if(attackerDice[0] > defenderDice[0])
        territories[dT].troops -= 1;
    else
        territories[aT].troops -= 1;
    if(attackers > 1 && defenders > 1)
    {
        if(attackerDice[1] > defenderDice[1])
            territories[dT].troops -= 1;
        else
            territories[aT].troops -= 1;
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
    if(!isNeighbor(source, t))
        return 0;
    return 1;
}
