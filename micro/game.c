// game.c:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "game.h"

// Global variables
int numPlayers;
int currentPlayer;
int selectedTerritorySource;
int selectedTerritoryDestination;
int attackerDice[3];
int defenderDice[2];
int nextCardTroops;

// Game variables that don't need to be exposed to the IO systems
State state;
int territoriesRemaining;

void initializeGame()
{
    currentPlayer = 0;
    selectedTerritorySource = -1;
    selectedTerritoryDestination = -1;
    attackerDice[0] = 0;
    attackerDice[1] = 0;
    attackerDice[2] = 0;
    defenderDice[0] = 0;
    defenderDice[1] = 0;
    nextCardTroops = 4;
    
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
        case ATTACK:
            declareAttack(input); break;
        case BATTLE:
            resolveBattle(input); break;
        case MOVE:
            moveTroops(input); break;
        case GAMEOVER:
            // TODO: do something here 
            break;
        default:
            ; // panic!
    }
}

void choosePlayers(Input input)
{
}

void selectTerritories(Input input)
{
}

void deployTroops(Input input)
{
}

void reinforce(Input input)
{
}

void declareAttack(Input input)
{
}

void resolveBattle(Input input)
{
}

void moveTroops(Input input)
{
}

int playerLiving(int player)
{
    return 0;
}

int canAttack(int territory)
{
    return 0;
}
