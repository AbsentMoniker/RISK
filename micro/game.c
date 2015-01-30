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
int currentPlayer;
int source;
int destination;
int attackerDice[3];
int defenderDice[2];
int nextCardTroopsIdx;

// Game variables that don't need to be exposed to the IO systems
static State state;
static int firstPlayer;
static int territoriesRemaining;
static int numTroops;

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
        case CONQUER:
            conquerTerritory(input); break;
        case MOVE1:
            moveTroops(input); break;
        case MOVE2:
            moveTroopsTarget(input); break;
        case MOVE3:
            moveTroopsNumber(input); break;
        case GAMEOVER:
            gameOver(input); break;
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
            setTextDisplay(0, "Choose number");
            setTextDisplay(1, "of players: %d", numPlayers);
            setTextDisplay(2, "A: Confirm number");
            setTextDisplay(3, "");
            break;
        case SELECT:
            setTextDisplay(0, "Player %d:", currentPlayer);
            setTextDisplay(1, "Pick territory");
            setTextDisplay(2, "A: Select", );
            setTextDisplay(3, "%d available", territoriesRemaining);
            break;
        case DEPLOY:
            setTextDisplay(0, "Player %d:", currentPlayer);
            setTextDisplay(1, "Deploy troop", );
            setTextDisplay(2, "A: Place troop");
            setTextDisplay(3, "%d troops left", numTroops);
            break;
        case REINFORCE:
            setTextDisplay(0, "Reinforcements", currentPlayer);
            setTextDisplay(1, "%d troops left", numTroops);
            setTextDisplay(2, "A: Place troop");
            setTextDisplay(3, "B: Other options");
            break;
        case ATTACK1:
            setTextDisplay(0, "Declare attacks");
            setTextDisplay(1, "");
            setTextDisplay(2, "A: Choose attacker");
            setTextDisplay(3, "B: Stop attacking");
            break;
        case ATTACK2:
            setTextDisplay(0, "Declare attacks");
            setTextDisplay(1, "(%s)", territories[source].name);
            setTextDisplay(2, "A: Choose target");
            setTextDisplay(3, "B: Back");
            break;
        case BATTLE:
            setTextDisplay(0, "%s vs", territories[source].name);
            setTextDisplay(1, "%s!", territories[destination].name);
            setTextDisplay(2, "A: Roll dice");
            setTextDisplay(3, "B: Retreat");
            break;
        case CONQUER:
            setTextDisplay(0, "Move troops to target");
            setTextDisplay(1, "");
            setTextDisplay(2, "A: Confirm troops");
            setTextDisplay(3, "");
            break;
        case MOVE1:
            setTextDisplay(0, "Make free move");
            setTextDisplay(1, "");
            setTextDisplay(2, "A: Choose source");
            setTextDisplay(3, "B: End turn");
            break;
        case MOVE2:
            setTextDisplay(0, "Make free move");
            setTextDisplay(1, "(%s)", territories[source].name);
            setTextDisplay(2, "A: Choose detination");
            setTextDisplay(3, "B: Back");
            break;
        case MOVE3:
            setTextDisplay(0, "%s -->", territories[source].name);
            setTextDisplay(1, "%s", territories[destination].name);
            setTextDisplay(2, "A: Confirm number");
            setTextDisplay(3, "B: Back");
            break;
        case GAMEOVER:
            setTextDisplay(0, "Player %d wins!", currentPlayer);
            setTextDisplay(1, "   R   I   S   K    ");
            setTextDisplay(2, "  C H A M P I O N   ");
            setTextDisplay(3, "B: Start new game");
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
        changeState(SELECT);
}

void selectTerritories(Input input)
{
    if(input == NEXT)
        moveSelection(0, 1, predUnowned);
    else if(input == PREVIOUS)
        moveSelection(0, -1, predUnowned);
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
            changeState(DEPLOY);
    }
}

void deployTroops(Input input)
{
    if(input == NEXT)
        moveSelection(0, 1, predOwnedCurrent);
    else if(input == PREVIOUS)
        moveSelection(0, -1, predOwnedCurrent);
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
            changeState(REINFORCE);
    }
}

void reinforce(Input input)
{
    if(input == NEXT)
        moveSelection(0, 1, predOwnedCurrent);
    else if(input == PREVIOUS)
        moveSelection(0, -1, predOwnedCurrent);
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;

        territories[destination].troops += 1;
        numTroops -= 1;

        if(numTroops == 0)
            changeState(ATTACK1);
    }
}

void declareAttack(Input input)
{
    if(input == NEXT)
        moveSelection(1, 1, predAttackSource);
    else if(input == PREVIOUS)
        moveSelection(1, -1, predAttackSource);
    else if(input == ADVANCE)
    {
        if(source == -1)
            return;
        changeState(ATTACK2);
    }
    else if(input == CANCEL)
        changeState(MOVE1);
}
void declareAttackTarget(Input input)
{
    if(input == NEXT)
        moveSelection(0, 1, predAttackTarget);
    else if(input == PREVIOUS)
        moveSelection(0, -1, predAttackTarget);
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;
        changeState(BATTLE);
    }
    else if(input == CANCEL)
        changeState(ATTACK1);
}

void resolveBattle(Input input)
{
    if(input == ADVANCE)
    {
        doBattle(source, destination);

        if(territories[source].troops == 1)
            changeState(ATTACK1);
        else if(territories[destination].troops == 0)
        {
            territories[destination].owner = currentPlayer;
            numTroops = min(3, territories[source].troops - 1);
            territories[destination].troops = 
                territories[source].troops - 1;
            territories[source].troops = 1;
            changeState(CONQUER);
        }
    }
    else if(input == CANCEL)
        changeState(ATTACK1);
      
}
void conquerTerritory(Input input)
{
    if(input == NEXT)
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
                territories[destination].troops > numTroops)
        {
            territories[destination].troops -= 1;
            territories[source].troops += 1;
        }
    }      
    else if(input == ADVANCE)
    {
        for(int i = 0; i < numPlayers; i++)
        {
            // Game continues unless current player is the only one left.
            if(i != currentPlayer && playerLiving(i))
            {
                changeState(ATTACK1);
                break;
            }
        }
        changeState(GAMEOVER);
    }
}


void moveTroops(Input input)
{
    if(input == NEXT)
        moveSelection(1, 1, predMoveSource);
    else if(input == PREVIOUS)
        moveSelection(1, -1, predMoveSource);
    else if(input == ADVANCE)
    {
        if(source == -1)
            return;
        changeState(MOVE2);
    }
    else if(input == CANCEL)
    {
        do
        {
            currentPlayer += 1;
            if(currentPlayer == numPlayers)
                currentPlayer = 0;
        } while(!playerLiving(currentPlayer));

        changeState(REINFORCE);
    }
}
void moveTroopsTarget(Input input)
{
    if(input == NEXT)
        moveSelection(0, 1, predMoveTarget);
    else if(input == PREVIOUS)
        moveSelection(0, -1, predMoveTarget);
    else if(input == ADVANCE)
    {
        if(destination == -1)
            return;
        changeState(MOVE3);
    }
    else if(input == CANCEL)
        changeState(MOVE1);
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
        
        changeState(REINFORCE);
    }
    else if(input == CANCEL)
    {
        territories[destination].troops -= 
            numTroops - territories[source].troops;
        territories[source].troops = numTroops;
        changeState(MOVE2);
    }
}
void gameOver(Input input)
{
    if(input == CANCEL)
    {
        changeState(INIT);
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

void changeState(State newstate)
{
    state = newstate;

    if(state == INIT)
    {
        resetGame();
        numPlayers = 2;
        nextCardTroopsIdx = 0;
        currentPlayer = -1;
    }

    if(state == SELECT)
    {
        territoriesRemaining = NUM_TERRITORIES;
        firstPlayer = randint(0, numPlayers - 1);
        currentPlayer = firstPlayer;
    }

    if(state != BATTLE && state != CONQUER && state != MOVE3)
    {
        destination = -1;
        if(state != ATTACK2 && state != MOVE2)
            source = -1;
    }

    if(state != ATTACK1 && state != BATTLE && state != CONQUER)
    {
        attackerDice[0] = attackerDice[1] = attackerDice[2] = -1;
        defenderDice[0] = defenderDice[1] = -1;
    }
    
    if(state == SELECT)
        numTroops = initialTroops[numPlayers];
    else if(state == REINFORCE)
        numTroops = computeReinforcements(currentPlayer);
    else if(state == MOVE3)
        numTroops = territories[source].troops;
    // numTroops is also used by CONQUER, but it needs to be set earlier than
    // this function is called
}

void resetGame()
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        territories[i].owner = -1;
        territories[i].troops = 0;
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
    int bonus = 0;

    for(int i = 0; i < NUM_TERRITORIES; i++)
        if(territories[i].owner == player)
            territoriesHeld += 1;

    for(int i = 0; i < NUM_CONTINENTS; i++)
    {
        int j;
        for(j = 0; j < continents[i].members; j++)
        {
            if(territories[j+continents[i].firstmember].owner != player)
                break;
        }
        if(j == continents[i].members)
            bonus += continents[i].value;
    }

    return bonus + max(3, territoriesHeld / 3);
}
