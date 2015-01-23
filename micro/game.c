// game.c:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "game.h"
#include "util.h"

// Global variables
int numPlayers;
int currentPlayer;
int source;
int destination;
int attackerDice[3];
int defenderDice[2];
int nextCardTroops;

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
        do
        {
            if(destination == NUM_TERRITORIES - 1)
                destination = 0;
            else
                destination += 1;
        } while(territories[destination].owner != -1);
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(destination <= 0)
                destination = NUM_TERRITORIES - 1;
            else
                destination -= 1;
        } while(territories[destination].owner != -1);
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
        do
        {
            if(destination == NUM_TERRITORIES - 1)
                destination = 0;
            else
                destination += 1;
        } while(territories[destination].owner != currentPlayer);
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(destination <= 0)
                destination = NUM_TERRITORIES - 1;
            else
                destination -= 1;
        } while(territories[destination].owner != currentPlayer);
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
        do
        {
            if(destination == NUM_TERRITORIES - 1)
                destination = 0;
            else
                destination += 1;
        } while(territories[destination].owner != currentPlayer);
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(destination <= 0)
                destination = NUM_TERRITORIES - 1;
            else
                destination -= 1;
        } while(territories[destination].owner != currentPlayer);
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
        do
        {
            if(source == NUM_TERRITORIES - 1)
                source = 0;
            else
                source += 1;
        } while(territories[source].owner != currentPlayer ||
                !canAttack(source));
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(source <= 0)
                source = NUM_TERRITORIES - 1;
            else
                source -= 1;
        } while(territories[source].owner != currentPlayer ||
                !canAttack(source));
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
        do
        {
            if(destination == NUM_TERRITORIES - 1)
                destination = 0;
            else
                destination += 1;
        } while(territories[destination].owner == currentPlayer ||
                !isNeighbor(source, destination));
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(destination <= 0)
                destination = NUM_TERRITORIES - 1;
            else
                destination -= 1;
        } while(territories[destination].owner == currentPlayer ||
                !isNeighbor(source, destination));
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
        do
        {
            if(source == NUM_TERRITORIES - 1)
                source = 0;
            else
                source += 1;
        } while(territories[source].owner != currentPlayer ||
                territories[source].troops == 1);
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(source <= 0)
                source = NUM_TERRITORIES - 1;
            else
                source -= 1;
        } while(territories[source].owner != currentPlayer ||
                territories[source].troops == 1);
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
        do
        {
            if(destination == NUM_TERRITORIES - 1)
                destination = 0;
            else
                destination += 1;
        } while(territories[destination].owner != currentPlayer ||
                !isNeighbor(source, destination));
    }
    else if(input == PREVIOUS)
    {
        do
        {
            if(destination <= 0)
                destination = NUM_TERRITORIES - 1;
            else
                destination -= 1;
        } while(territories[destination].owner != currentPlayer ||
                !isNeighbor(source, destination));
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
        territories[destination].troops -= numTroops - territories[source].troops;
        territories[source].troops = numTroops;

        state = MOVE2;
    }
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

int canAttack(int territory)
{
    if(territories[territory].troops == 1)
        return 0;
    for(int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if(territories[territory].neighbors[i] != -1 &&
                territories[territories[territory].neighbors[i]].owner !=
                territories[territory].owner)
        {
            return 1;
        }
    }
    return 0;
}

int isNeighbor(int territory1, int territory2)
{
    for(int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if(territories[territory1].neighbors[i] == territory2)
        {
            return 1;
        }
    }
    return 0;
}
