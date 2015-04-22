#ifndef GAME_H
#define GAME_H
// game.h:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "types.h"

// Game options. These are determined by setting options during game startup
// and do not change during the course of a game.
extern int randomDeploy;
extern int numPlayers;
extern int multipleDeploy;
extern int useNeutrals;
extern CardValueScheme cardValueScheme;
#define OPTION_NUM_PLAYERS 0
#define OPTION_CARD_SCHEME 1
#define OPTION_RANDOM_DEPLOY 2
#define OPTION_MULTIPLE_DEPLOY 3
#define OPTION_USE_NEUTRALS 4

#define OPTION_END_GAME 5
#define OPTION_MAKE_TRADE 6

#define RANDOM_DEPLOY_NO 2
#define RANDOM_DEPLOY_TERRITORIES 1
#define RANDOM_DEPLOY_FULL 0

#define NEUTRAL_PLAYER 2

// Global variables holding the current gamestate. These are all things that
// the display routines need to know about to output information to the LEDs,
// in addition to the territories array in gamedata.c
extern int currentPlayer;
extern int source;
extern int destination;
extern int attackerDice[3];
extern int defenderDice[2];
extern int continentOwners[NUM_CONTINENTS];

// More gamestate. These do not need to be displayed in any way, but to need to
// be read/written when a game is restored or saved.
extern State state;
extern int firstPlayer;
extern int territoriesRemaining;
extern int numTroops;
extern int deployTroopsLeft[MAX_PLAYERS];
extern int mustTrade;
extern int needCard;
extern int currentOption;
extern int reinforceMenu;
extern int confirm;

// gameInput is the main entry point into the game logic; this function calls
// one of the other functions below it to advance the game state.
void gameInput(Input input);
// cardInput is the other type of input, a command from the web server that
// the current player wishes to exchange cards. Returns false if for some
// reason the exchange could not actually happen.
int cardInput(int card1, int card2, int card3);
int cardInputHand(int card1, int card2, int card3);
// updateText is called after every game update to update the text displayed on
// the LCD
void updateText();

// These functions actually perform the bulk of the game logic; there is one
// function for each possible GameState value, and each responds to the four
// possible inputs in some way.
void chooseOptions(Input input);
void selectTerritories(Input input);
void deployTroops(Input input);
void reinforce(Input input);
void declareAttack(Input input);
void declareAttackTarget(Input input);
void resolveBattle(Input input);
void conquerTerritory(Input input);
void moveTroops(Input input);
void moveTroopsTarget(Input input);
void moveTroopsNumber(Input input);
void gameOver(Input input);

// Helper functions for the game logic; these are functions that get called
// from many of the above update functions.
void moveSelection(int movesource, int direction, int (*predicate)(int));
void changeState(State state);

// Reset the game state (territories, cards, etc.) to their initial values, in
// preparation for starting a new game.
void resetGame();

// Attempts to make a trade using the cards the player has in hand.
void attemptTrade();

// playerLiving is true if player owns at least one territory; dead players do
// not get turns
int playerLiving(int player);
// computeReinforcements includes the number of reinforcements a player get
// from territory count and continent bonuses; it does not include card
// exchanging.
int computeReinforcements(int player);
// updateContinents updates the current owner, if any, of each continent. It
// should be called by computeReinforcements, but may also be called by the I/O
// whenever.
void updateContinents();

// Randomly allocate territories to the players, in effect skipping the SELECT
// state.
void allocateRandomTerritories();

// Randomly deploy troops, in effect skipping the DEPLOY state (except the
// final confirmation of game start).
void deployRandom();

// Select a currently unoccupied territory for the neutral armies.
void selectNeutralTerritory();

#endif
