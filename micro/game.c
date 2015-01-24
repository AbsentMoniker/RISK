// game.c:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "game.h"
#include "types.h"
#include "territory.h"
#include "gamedata.h"
#include "io.h"
#include "util.h"

// Global variables
int numPlayers;
int firstPlayer;
int currentPlayer;
int source;
int destination;
int attackerDice[3];
int defenderDice[2];
int nextCardTroops;
int numTroops;

// Game variables that don't need to be exposed to the IO systems
State state;
int territoriesRemaining;

void initializeGame()
{
    numPlayers = 2;
    firstPlayer = 0;
    currentPlayer = 0;
    source = -1;
    destination = -1;
    attackerDice[0] = 0;
    attackerDice[1] = 0;
    attackerDice[2] = 0;
    defenderDice[0] = 0;
    defenderDice[1] = 0;
    nextCardTroops = 4;
    numTroops = 0;
    
    state = INIT;
    territoriesRemaining = NUM_TERRITORIES;
    updateText();
}

void gameInput(Input input)
{
    switch(state)
    {
        case INIT:
            choosePlayers(input); break;
        case SELECT:
            selectTerritories(input); break;
        case DEPLOY:
            deployTroops(input); break;
        case REINFORCE:
            reinforce(input); break;
        case ATTACK1:
            declareAttack(input); break;
        case ATTACK2:
            declareAttackTarget(input); break;
        case BATTLE:
            resolveBattle(input); break;
        case MOVE1:
            moveTroops(input); break;
        case MOVE2:
            moveTroopsTarget(input); break;
        case MOVE3:
            moveTroopsNumber(input); break;
        case GAMEOVER:
            // TODO: do something here 
            break;
        default:
            ; // panic!
    }
    updateText();
}

void updateText()
{
    switch(state)
    {
        case INIT:
            setTextDisplay(0, "Num. players:");
            setTextDisplay(1, "%d", numPlayers);
            break;
        case SELECT:
            setTextDisplay(0, "Pick territory");
            setTextDisplay(1, "%d left", territoriesRemaining);
            break;
        case DEPLOY:
            setTextDisplay(0, "Ply %d deploy", currentPlayer);
            setTextDisplay(1, "%d troops left", numTroops);
            break;
        case REINFORCE:
            setTextDisplay(0, "Reinforcements", currentPlayer);
            setTextDisplay(1, "%d troops left", numTroops);
            break;
        case ATTACK1:
            setTextDisplay(0, "Choose attacker");
            setTextDisplay(1, "or end turn");
            break;
        case ATTACK2:
            setTextDisplay(0, "Choose target");
            setTextDisplay(1, "");
            break;
        case BATTLE:
            setTextDisplay(0, "Battle or");
            setTextDisplay(1, "retreat");
            break;
        case MOVE1:
            setTextDisplay(0, "Make free move");
            setTextDisplay(1, "or end turn");
            break;
        case MOVE2:
            setTextDisplay(0, "Choose");
            setTextDisplay(1, "destination");
            break;
        case MOVE3:
            setTextDisplay(0, "Number to move");
            setTextDisplay(1, "");
            break;
        case GAMEOVER:
            break;
        default:
            ; // panic!
    }
}

void choosePlayers(Input input)
{
    if(input == NEXT && numPlayers < MAX_PLAYERS)
        numPlayers += 1;
    else if(input == PREVIOUS && numPlayers > 2)
        numPlayers -= 1;
    else if(input == ADVANCE)
    {
        state = SELECT;
        firstPlayer = 0; // TODO: make this random
        currentPlayer = firstPlayer;
        numTroops = initialTroops[numPlayers];
    }
}

void selectTerritories(Input input)
{
    if(input == NEXT)
    {
        moveSelection(0, 1, predUnowned);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(0, -1, predUnowned);
    }
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;

        territories[destination].owner = currentPlayer;
        territories[destination].troops = 1;
        destination = -1;

        currentPlayer += 1;
        if(currentPlayer == numPlayers)
            currentPlayer = 0;

        territoriesRemaining -= 1;
        if(currentPlayer == firstPlayer)
            numTroops -= 1;

        if(territoriesRemaining == 0)
        {
            state = DEPLOY;
        }
    }
}

void deployTroops(Input input)
{
    if(input == NEXT)
    {
        moveSelection(0, 1, predOwnedCurrent);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(0, -1, predOwnedCurrent);
    }
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;

        territories[destination].troops += 1;
        destination = -1;

        currentPlayer += 1;
        if(currentPlayer == numPlayers)
            currentPlayer = 0;

        if(currentPlayer == firstPlayer)
            numTroops -= 1;

        if(numTroops == 0)
        {
            // The game is afoot...
            state = REINFORCE;
            numTroops = computeReinforcements(currentPlayer);
        }
    }
}

void reinforce(Input input)
{
    if(input == NEXT)
    {
        moveSelection(0, 1, predOwnedCurrent);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(0, -1, predOwnedCurrent);
    }
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;

        territories[destination].troops += 1;
        numTroops -= 1;

        if(numTroops == 0)
        {
            state = ATTACK1;
            source = -1;
            destination = -1;
        }
    }
}

void declareAttack(Input input)
{
    if(input == NEXT)
    {
        moveSelection(1, 1, predAttackSource);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(1, -1, predAttackSource);
    }
    else if(input == ADVANCE)
    {
        if(source == -1)
            return;

        state = ATTACK2;
        destination = -1;
    }
    else if(input == CANCEL)
    {
        state = MOVE1;
        source = -1;
        destination = -1;
    }

}
void declareAttackTarget(Input input)
{
    if(input == NEXT)
    {
        moveSelection(0, 1, predAttackTarget);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(0, -1, predAttackTarget);
    }
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;

        state = BATTLE;
    }
    else if(input == CANCEL)
    {
        destination = -1;
        state = ATTACK1;
    }
}

void resolveBattle(Input input)
{
    static int mintroops;
    if(input == ADVANCE)
    {
        if(territories[destination].owner != currentPlayer)
        {
            doBattle(source, destination);
            if(territories[source].troops == 1)
            {
                state = ATTACK1;
                source = -1;
                destination = -1;
            }
            if(territories[destination].troops == 0)
            {
                territories[destination].owner = currentPlayer;
                mintroops = min(3, territories[source].troops - 1);
                territories[source].troops -= mintroops;
                territories[destination].troops += mintroops;
            }
        }
        else
        {
            state = ATTACK1;
            source = -1;
            destination = -1;
        }
    }
    else if(input == CANCEL)
    {
        if(territories[destination].owner != currentPlayer)
        {
            state = ATTACK1;
            source = -1;
            destination = -1;
        }
    }
    else if(input == NEXT)
    {
        if(territories[destination].owner == currentPlayer &&
                territories[source].troops > 1)
        {
            territories[destination].troops += 1;
            territories[source].troops -= 1;
        }
    }
    else if(input == PREVIOUS)
    {
        if(territories[destination].owner == currentPlayer &&
                territories[destination].troops > mintroops)
        {
            territories[destination].troops -= 1;
            territories[source].troops += 1;
        }
    }            
}

void moveTroops(Input input)
{
    if(input == NEXT)
    {
        moveSelection(1, 1, predMoveSource);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(1, -1, predMoveSource);
    }
    else if(input == ADVANCE)
    {
        if(source == -1)
            return;

        state = MOVE2;
        destination = -1;
    }
    else if(input == CANCEL)
    {
        do
        {
            currentPlayer += 1;
            if(currentPlayer == numPlayers)
                currentPlayer = 0;
        } while(!playerLiving(currentPlayer));

        state = REINFORCE;
        numTroops = computeReinforcements(currentPlayer);
        source = -1;
        destination = -1;
    }
}
void moveTroopsTarget(Input input)
{
    if(input == NEXT)
    {
        moveSelection(0, 1, predMoveTarget);
    }
    else if(input == PREVIOUS)
    {
        moveSelection(0, -1, predMoveTarget);
    }
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;
        numTroops = territories[source].troops;

        state = MOVE3;
    }
    else if(input == CANCEL)
    {
        destination = -1;
        state = MOVE1;
    }
}
void moveTroopsNumber(Input input)
{
    if(input == NEXT)
    {
        if(territories[source].troops > 1)
        {
            territories[destination].troops += 1;
            territories[source].troops -= 1;
        }
    }
    else if(input == PREVIOUS)
    {
        if(territories[source].troops < numTroops)
        {
            territories[destination].troops -= 1;
            territories[source].troops += 1;
        }
    }
    else if(input == ADVANCE)
    {
        do
        {
            currentPlayer += 1;
            if(currentPlayer == numPlayers)
                currentPlayer = 0;
        } while(!playerLiving(currentPlayer));

        state = REINFORCE;
        numTroops = computeReinforcements(currentPlayer);
        source = -1;
        destination = -1;
    }
    else if(input == CANCEL)
    {
        territories[destination].troops -= 
            numTroops - territories[source].troops;
        territories[source].troops = numTroops;

        state = MOVE2;
    }
}

void moveSelection(int movesource, int direction, int (*predicate)(int))
{
    int * p = (movesource? &source : &destination);

    int tries = 0;
    while(tries < NUM_TERRITORIES)
    {
        *p += direction;
        if(*p < 0)
            *p = NUM_TERRITORIES - 1;
        else if(*p >= NUM_TERRITORIES)
            *p = 0;

        tries++;
        if(predicate(*p))
            return;
    }
    // Couldn't find a valid territory, so make sure nothing is selected
    *p = -1; 
}

int playerLiving(int player)
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
        if(territories[i].owner == player)
            return 1;
    return 0;
}

int computeReinforcements(int player)
{
    int territoriesHeld = 0;
    for(int i = 0; i < NUM_TERRITORIES; i++)
        if(territories[i].owner == player)
            territoriesHeld += 1;
    // TODO: continents
    return max(3, territoriesHeld / 3);
}


