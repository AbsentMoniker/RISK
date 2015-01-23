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
    numPlayers = 2;
    firstPlayer = 0;
    currentPlayer = 0;
    selectedTerritorySource = -1;
    selectedTerritoryDestination = -1;
    attackerDice[0] = 0;
    attackerDice[1] = 0;
    attackerDice[2] = 0;
    defenderDice[0] = 0;
    defenderDice[1] = 0;
    nextCardTroops = 4;
    troopsRemaining = 0;
    
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
    if(input == NEXT && numPlayers < MAX_PLAYERS)
        numPlayers += 1;
    else if(input == PREVIOUS && numPlayers > 2)
        numPlayers -= 1;
    else if(input == ADVANCE)
    {
        state = SELECT;
        firstPlayer = 0; // TODO: make this random
        currentPlayer = firstPlayer;
        troopsRemaining = initialTroops[numPlayers];
    }
}

void selectTerritories(Input input)
{
    if(input == NEXT)
    {
        if(selectedTerritoryDestination == NUM_TERRITORIES - 1)
            selectedTerritoryDestination = -1;

        do
        {
            selectedTerritoryDestination += 1;
        } while(territories[selectedTerritoryDestination].owner != -1);
    }
    else if(input == PREVIOUS)
    {
        if(selectedTerritoryDestination <= 0)
            selectedTerritoryDestination = NUM_TERRITORIES;

        do
        {
            selectedTerritoryDestination -= 1;
        } while(territories[selectedTerritoryDestination].owner != -1);
    }
    else if(input == ADVANCE)
    {
        if(selectedTerritoryDestination == -1)
            return;

        territories[selectedTerritoryDestination].owner = currentPlayer;
        territories[selectedTerritoryDestination].troops = 1;
        selectedTerritoryDestination = -1;

        currentPlayer += 1;
        if(currentPlayer == NUM_PLAYERS)
            currentPlayer = 0;

        territoriesRemaining -= 1;
        if(currentPlayer == firstPlayer)
            troopsRemaining -= 1;

        if(territoriesRemaining == 0)
        {
            state = DEPLOY;
        }
    }
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
