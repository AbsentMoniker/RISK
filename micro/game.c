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
#include "cards.h"
#include "log.h"
// Global variables
int numPlayers;
int randomTerritories;
// cardValueScheme is handled by cards.c

int currentPlayer;
int source;
int destination;
int attackerDice[3];
int defenderDice[2];

// Game variables that don't need to be exposed to the IO systems
static State state;
static int firstPlayer;
static int territoriesRemaining;
static int numTroops;
static int mustTrade;
static int needCard;
static int currentOption;
static int reinforceMenu;
static int confirm;

void gameInput(Input input)
{
    switch(state)
    {
        case INIT:
            chooseOptions(input); break;
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

int cardInput(int card1, int card2, int card3)
{
    if(state != REINFORCE)
        return 0;
    int troops = exchangeCards(currentPlayer, card1, card2, card3);
    numTroops += troops;
    if(hands[currentPlayer].cards < 5)
        mustTrade = 0;
    updateText();
    return troops;
}

void updateText()
{
    switch(state)
    {
        case INIT:
            if(currentOption == OPTION_NUM_PLAYERS)
            {
                setTextDisplay(0, "Number of players");
                setTextDisplay(1, "%d", numPlayers);
            }
            else if(currentOption == OPTION_CARD_SCHEME)
            {
                setTextDisplay(0, "Card exchange values");
                if(cardValueScheme == INCREASING)
                    setTextDisplay(1, "Increasing");
                else if(cardValueScheme == INCREASING_ONE)
                    setTextDisplay(1, "Increasing by 1");
                else if(cardValueScheme == SET_VALUE)
                    setTextDisplay(1, "Set Value");
            }
            else if(currentOption == OPTION_RANDOM_TERRITORIES)
            {
                setTextDisplay(0, "Territory allocation");
                if(randomTerritories)
                    setTextDisplay(1, "Random");
                else
                    setTextDisplay(1, "Select");
            }
            setTextDisplay(2, "A: Start game");
            setTextDisplay(3, "B: Next option");
            break;
        case SELECT:
            setTextDisplay(0, "Player %d:", currentPlayer);
            setTextDisplay(1, "Pick territory");
            setTextDisplay(2, "A: Select");
            setTextDisplay(3, "%d available", territoriesRemaining);
            break;
        case DEPLOY:
            if(!confirm)
            {
                setTextDisplay(0, "Player %d:", currentPlayer);
                setTextDisplay(1, "Deploy troops");
                setTextDisplay(2, "A: Place troop");
                setTextDisplay(3, "%d troops left", numTroops);
            }
            else
            {
                setTextDisplay(0, "All troops have been");
                setTextDisplay(1, "deployed.");
                setTextDisplay(2, "");
                setTextDisplay(3, "B: Begin play");
            }
            break;
        case REINFORCE:
            if(reinforceMenu && !confirm)
            {
                setTextDisplay(0, "Options:");
                setTextDisplay(1, "End game?");
                setTextDisplay(2, "A: End game");
                setTextDisplay(3, "B: Cancel");
            }
            else if(reinforceMenu && confirm)
            {
                setTextDisplay(0, "Really end game?");
                setTextDisplay(1, "");
                setTextDisplay(2, "A: Yes");
                setTextDisplay(3, "B: No");
            }
            else if(numTroops == 0 && mustTrade)
            {
                setTextDisplay(0, "%d cards in hand", 
                        hands[currentPlayer].cards);
                setTextDisplay(1, "You must exchange");
                setTextDisplay(2, "cards to continue");
                setTextDisplay(3, "B: Other options");            
            }
            else
            {
                setTextDisplay(0, "Reinforcements");
                setTextDisplay(1, "%d troops left", numTroops);
                setTextDisplay(2, "A: Place troop");
                setTextDisplay(3, "B: Other options");
            }
            break;
        case ATTACK1:
            if(!confirm)
            {
                setTextDisplay(0, "Declare attacks");
                setTextDisplay(1, "");
                setTextDisplay(2, "A: Choose attacker");
                setTextDisplay(3, "B: Stop attacking");
            }
            else
            {
                setTextDisplay(0, "Really stop");
                setTextDisplay(1, "attacking?");
                setTextDisplay(2, "A: Yes");
                setTextDisplay(3, "B: No");
            }
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
            setTextDisplay(0, "Move troops into");
            setTextDisplay(1, "%s", territories[destination].name);
            setTextDisplay(2, "A: Confirm troops");
            setTextDisplay(3, "");
            break;
        case MOVE1:
            if(!confirm)
            {
                setTextDisplay(0, "Make free move");
                setTextDisplay(1, "");
                setTextDisplay(2, "A: Choose source");
                setTextDisplay(3, "B: End turn");
            }
            else
            {
                setTextDisplay(0, "Really end turn");
                setTextDisplay(1, "without a move?");
                setTextDisplay(2, "A: Yes");
                setTextDisplay(3, "B: No");
            }
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

void chooseOptions(Input input)
{
    if(input == ADVANCE)
    {
        changeState(SELECT);
        if(randomTerritories)
        {
            allocateRandomTerritories();
            changeState(DEPLOY);
        }
        return;
    }
    if(currentOption == OPTION_NUM_PLAYERS)
    {
        if(input == NEXT && numPlayers < MAX_PLAYERS)
            numPlayers += 1;
        else if(input == PREVIOUS && numPlayers > 2)
            numPlayers -= 1;
        else if(input == CANCEL)
            currentOption = OPTION_RANDOM_TERRITORIES;
    }
    else if(currentOption == OPTION_RANDOM_TERRITORIES)
    {
        if(input == NEXT || input == PREVIOUS)
            randomTerritories = !randomTerritories;
        else if(input == CANCEL)
            currentOption = OPTION_CARD_SCHEME;
    }
    else if(currentOption == OPTION_CARD_SCHEME)
    {
        if(input == NEXT)
            cardValueScheme = (cardValueScheme == 2? 0 : cardValueScheme + 1);
        else if(input == PREVIOUS)
            cardValueScheme = (cardValueScheme == 0? 2 : cardValueScheme - 1);
        else if(input == CANCEL)
            currentOption = OPTION_NUM_PLAYERS;
    }
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

        // Changing the state here after random allocation would cause
        // recursion, so the function for the INIT state handles the state
        // change instead right after the random deployment. 
        if(territoriesRemaining == 0 && !randomTerritories)
            changeState(DEPLOY);
    }
}

void deployTroops(Input input)
{
    if(input == NEXT && !confirm)
        moveSelection(0, 1, predOwnedCurrent);
    else if(input == PREVIOUS && !confirm)
        moveSelection(0, -1, predOwnedCurrent);
    else if(input == ADVANCE)
    {
        if(confirm)
            return;
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
            confirm = 1;
    }
    else if(input == CANCEL)
    {
        if(confirm)
        {
            for(int i = 0; i < NUM_TERRITORIES; i++)
                logReinforce(territories[i].owner, i, territories[i].troops);
            changeState(REINFORCE);
        }
    }
}

void reinforce(Input input)
{
    if(input == NEXT && !reinforceMenu)
        moveSelection(0, 1, predOwnedCurrent);
    else if(input == PREVIOUS && !reinforceMenu)
        moveSelection(0, -1, predOwnedCurrent);
    else if(input == ADVANCE)
    {
        if(reinforceMenu)
        {
            if(!confirm)
                confirm = 1;
            else
                changeState(INIT);
            return;
        }

        if(destination == -1)
            return;

        if(numTroops > 0)
        {
            logReinforce(currentPlayer, destination, 1);
            territories[destination].troops += 1;
            numTroops -= 1;
        }

        if(numTroops == 0 && !mustTrade)
        {
            logReinforce(currentPlayer, -1, 0);
            changeState(ATTACK1);
        }
    }
    else if(input == CANCEL)
    {
        if(reinforceMenu && confirm)
            confirm = 0;
        else if(reinforceMenu)
            reinforceMenu = 0;
        else
            reinforceMenu = 1;
    }
}

void declareAttack(Input input)
{
    if(input == NEXT && !confirm)
        moveSelection(1, 1, predAttackSource);
    else if(input == PREVIOUS && !confirm)
        moveSelection(1, -1, predAttackSource);
    else if(input == ADVANCE)
    {
        if(confirm)
        {
            if(needCard)
                drawCard(currentPlayer);
            changeState(MOVE1);
            return;
        }

        if(source == -1)
            return;
        changeState(ATTACK2);
    }
    else if(input == CANCEL)
    {
        confirm = !confirm;
    }
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
        logAttack(currentPlayer, territories[destination].owner, source,
                destination);
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
            int defeatedPlayer = territories[destination].owner;
            territories[destination].owner = currentPlayer;

            numTroops = min(3, territories[source].troops - 1);
            territories[destination].troops = 
                territories[source].troops - 1;
            territories[source].troops = 1;

            if(!playerLiving(defeatedPlayer))
            {
                takeHand(currentPlayer, defeatedPlayer);
                if(hands[currentPlayer].cards >= 5)
                    mustTrade = 1;
            }
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
        if(territories[source].troops > 1)
        {
            territories[destination].troops += 1;
            territories[source].troops -= 1;
        }
    }
    else if(input == PREVIOUS)
    {
        if(territories[destination].troops > numTroops)
        {
            territories[destination].troops -= 1;
            territories[source].troops += 1;
        }
    }      
    else if(input == ADVANCE)
    {
        needCard = 1;
        logConquer(currentPlayer, destination, territories[destination].troops);
        for(int i = 0; i < numPlayers; i++)
        {
            // Game continues unless current player is the only one left.
            if(i != currentPlayer && playerLiving(i))
            {
                if(mustTrade)
                    changeState(REINFORCE);
                else
                    changeState(ATTACK1);
                return;
            }
        }
        changeState(GAMEOVER);
    }
}


void moveTroops(Input input)
{
    if(input == NEXT && !confirm)
        moveSelection(1, 1, predMoveSource);
    else if(input == PREVIOUS && !confirm)
        moveSelection(1, -1, predMoveSource);
    else if(input == ADVANCE)
    {
        if(confirm)
        {
            do
            {
                currentPlayer += 1;
                if(currentPlayer == numPlayers)
                    currentPlayer = 0;
            } while(!playerLiving(currentPlayer));

            changeState(REINFORCE);
            return;
        }

        if(source == -1)
            return;

        computeConnected(source);
        changeState(MOVE2);
    }
    else if(input == CANCEL)
    {
        confirm = !confirm;
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
        logMove(currentPlayer, source, destination, 
                numTroops - territories[source].troops);

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
        cardValueScheme = INCREASING;
        randomTerritories = 1;
        currentOption = OPTION_NUM_PLAYERS; 

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
        attackerDice[0] = attackerDice[1] = attackerDice[2] = 0;
        defenderDice[0] = defenderDice[1] = 0;
    }
    
    if(state == SELECT)
        numTroops = initialTroops[numPlayers];
    else if(state == REINFORCE)
    {
        if(mustTrade)
            numTroops = 0;
        else
            numTroops = computeReinforcements(currentPlayer);
    }
    else if(state == MOVE3)
        numTroops = territories[source].troops;
    // numTroops is also used by CONQUER, but it needs to be set earlier than
    // this function is called

    if(state == REINFORCE)
    {
        reinforceMenu = 0;
        if(hands[currentPlayer].cards >= 5)
            mustTrade = 1;
    }
    else if(state != CONQUER)
        mustTrade = 0;

    if(state == REINFORCE && !mustTrade)
        needCard = 0;

    confirm = 0;
}

void resetGame()
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        territories[i].owner = -1;
        territories[i].troops = 0;
    }
    initCards();
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

#define SWAP(a,b) do{int tmp = a; a = b; b = tmp;}while(0)
void allocateRandomTerritories()
{
    // create a list and randomize it
    int list[NUM_TERRITORIES];
    for(int i = 0; i < NUM_TERRITORIES; i++)
        list[i] = i;
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        int r = randint(i, NUM_TERRITORIES - 1);
        SWAP(list[i], list[r]);
    }

    // deal territories by forcing inputs
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        destination = list[i];
        selectTerritories(ADVANCE);
    }
}
