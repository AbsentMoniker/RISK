// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#include <SFML/Graphics.hpp>
extern "C" {
#include "../game.h"
#include "../gamedata.h"
#include "../types.h"
#include "../territory.h"
#include "../io.h"
#include "../cards.h"
#include "../log.h"
#include "../save.h"
#include "../stats.h"
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <random>
#include <ctime>

std::mt19937 rng;

// Implement the io functions needed by io.h
extern "C" {
    char textdisplay[4][21] = {0};
    void setTextDisplay(int line, const char * format, ...)
    {
        if(line < 0 || line >= 4)
            return;

        va_list args;
        va_start(args, format);
        char str[21];
        vsnprintf(str, 21, format, args);
        va_end(args);

        //                          01234567890123456789
        strncpy(textdisplay[line], "                    ", 21);
        strncpy(textdisplay[line], str, 21);
    }

    int randint(int min, int max)
    {
        return std::uniform_int_distribution<int>(min, max)(rng);
    }

    void panic(int line, const char * file, const char * fun, const char * text)
    {
        fprintf(stderr, "Error in %s, %s line %d: %s\n",
                fun, file, line, text);
        abort();
    }
}


sf::Vector2f terPositions[NUM_TERRITORIES] =
{
#ifdef USE_SIMPLE_MAP
    {10,10},
    {10,60},
#else
    {280,220},//[T_NORTHERN_AFRICA]  0
    {150,300},//[T_BRAZIL]  1
    {120,350},//[T_ARGENTINA]  2
    {100,300},//[T_PERU]  3
    {120,250},//[T_VENEZUELA]  4
    {90,160},//[T_MEXICO]  5
    {120,110},//[T_EASTERN_UNITED_STATES]  6
    {70,110},//[T_WESTERN_UNITED_STATES]  7
    {10,10},//[T_ALASKA]  8
    {60,10},//[T_NORTHWEST]  9
    {60,60},//[T_ALBERTA]  10
    {110,60},//[T_ONTARIO]  11
    {160,60},//[T_QUEBEC]  12
    {140,10},//[T_GREENLAND]  13
    {250,10},//[T_ICELAND]  14
    {320,10},//[T_SCANDINAVIA]  15
    {380,70},//[T_UKRAINE]  16
    {320,60},//[T_NORTHERN_EUROPE] 17
    {270,60},//[T_GREAT_BRITAIN]  18
    {280,120},//[T_WESTERN_EUROPE]  19
    {340,120},//[T_SOUTHERN_EUROPE]  20
    {330,210},//[T_EGYPT]  21
    {330,260},//[T_CONGO]  22
    {330,310},//[T_SOUTH_AFRICA]  23
    {380,310},//[T_MADAGASCAR]  24
    {620,130},//[T_JAPAN]  24
    {380,250},//[T_EASTERN_AFRICA]  25
    {460,180},//[T_MIDDLE_EAST]  26
    {470,130},//[T_AFGHANISTAN]  27
    {440,50},//[T_URALS]  28
    {490,50},//[T_SIBERIA]  29
    {540,20},//[T_YAKUTSK]  30
    {600,20},//[T_KAMCHATKA]  31
    {540,70},//[T_IRKUTSK]  32
    {560,130},//[T_MONGOLIA]  33
    {560,180},//[T_CHINA]  35
    {540,230},//[T_INDIA]  36
    {590,230},//[T_SIAM]  37
    {580,340},//[T_INDONESIA]  38
    {640,340},//[T_NEW_GUINEA]  39
    {590,400},//[T_WESTERN_AUSTRALIA]  40
    {640,400},//[T_EASTERN_AUSTRALIA]  41
#endif
};
sf::Color playerColors[MAX_PLAYERS] =
{
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Yellow,
#ifndef USE_SIMPLE_MAP
    sf::Color::Blue,
    sf::Color::Magenta,
    sf::Color::Cyan,
#endif
};

sf::Font font;
sf::Clock blinkclock;

void drawMap(sf::RenderWindow & win)
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        sf::RectangleShape shape(sf::Vector2f(46, 46));
        shape.setPosition(terPositions[i]);

        
        if((i == source || i == destination) &&
                blinkclock.getElapsedTime() > sf::milliseconds(125))
        {
            shape.setFillColor(sf::Color::White);
        }
        else if(territories[i].owner == -1)
        {
            shape.setFillColor(sf::Color::Black);
        } 
        else
        {
            shape.setFillColor(playerColors[territories[i].owner]);
        }

        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        win.draw(shape);

        sf::Text troops(std::to_string(territories[i].troops), font, 20);
        troops.setColor(sf::Color::Black);
        troops.setPosition(terPositions[i] + sf::Vector2f(10, 15));
        win.draw(troops);
    }

}

void drawText(sf::RenderWindow & win)
{
    sf::Text line(textdisplay[0], font, 20);
    line.setColor(sf::Color::White);
    line.setPosition(sf::Vector2f(20, 460));
    win.draw(line);

    line.setString(textdisplay[1]);
    line.setPosition(sf::Vector2f(20, 490));
    win.draw(line);

    line.setString(textdisplay[2]);
    line.setPosition(sf::Vector2f(20, 520));
    win.draw(line);

    line.setString(textdisplay[3]);
    line.setPosition(sf::Vector2f(20, 550));
    win.draw(line);
}

void drawDie(sf::RenderWindow & win, int d, int x, int y)
{
    if(d == 0)
        return;
    sf::Text num(std::to_string(d), font, 24);
    num.setColor(sf::Color::Red);
    num.setPosition(x,y);
    win.draw(num);
}

void dumpLog()
{
    int count[numPlayers][7] = {0};
    for(int i = 0; i < gamelogSize; i++)
    {
        printf("%08x    ", ((unsigned *)gamelog)[i]);
        if(gamelog[i].type == LOG_REINFORCE)
        {
            printf("Player %d: %d troops placed in %s\n",
                    gamelog[i].reinforce.player,
                    gamelog[i].reinforce.troops,
                    territories[gamelog[i].reinforce.territory].name);
        }
        else if(gamelog[i].type == LOG_ATTACK)
        {
            printf("%s (%d) attacks %s (%d)\n",
                    territories[gamelog[i].attack.attackingTerritory].name,
                    gamelog[i].attack.attackingPlayer,
                    territories[gamelog[i].attack.defendingTerritory].name,
                    gamelog[i].attack.defendingPlayer);
        }
        else if(gamelog[i].type == LOG_BATTLE)
        {
            count[gamelog[i].battle.attackingPlayer]
                [gamelog[i].battle.aDie1]++;
            count[gamelog[i].battle.attackingPlayer]
                [gamelog[i].battle.aDie2]++;
            count[gamelog[i].battle.attackingPlayer]
                [gamelog[i].battle.aDie3]++;
            count[gamelog[i].battle.defendingPlayer]
                [gamelog[i].battle.dDie1]++;
            count[gamelog[i].battle.defendingPlayer]
                [gamelog[i].battle.dDie2]++;
            printf("A %d: %d %d %d    D %d: %d %d\n",
                    gamelog[i].battle.attackingPlayer,
                    gamelog[i].battle.aDie1,
                    gamelog[i].battle.aDie2,
                    gamelog[i].battle.aDie3,
                    gamelog[i].battle.defendingPlayer,
                    gamelog[i].battle.dDie1,
                    gamelog[i].battle.dDie2);
        }
        else if(gamelog[i].type == LOG_CONQUER)
        {
            printf("Player %d takes %s with %d troops\n",
                    gamelog[i].conquer.attackingPlayer,
                    territories[gamelog[i].conquer.territory].name,
                    gamelog[i].conquer.troops);
        }
        else if(gamelog[i].type == LOG_MOVE)
        {
            printf("Player %d moves %d troops from %s to %s\n",
                    gamelog[i].move.player,
                    gamelog[i].move.troops,
                    territories[gamelog[i].move.sourceTerritory].name,
                    territories[gamelog[i].move.destinationTerritory].name);
        }
        else if(gamelog[i].type == LOG_CARD_GIVEN)
        {
            printf("Player %d receives card %d (%s, type %d)\n",
                    gamelog[i].cardgiven.player,
                    gamelog[i].cardgiven.territory,
                    gamelog[i].cardgiven.territory != -1?
                        territories[gamelog[i].cardgiven.territory].name :
                        "wild",
                    gamelog[i].cardgiven.cardtype);
        }
        else if(gamelog[i].type == LOG_CARD_EXCHANGE)
        {
            printf("Player %d exchanges %d %d %d for %d troops\n",
                    gamelog[i].exchange.player,
                    gamelog[i].exchange.cardtype1,
                    gamelog[i].exchange.cardtype2,
                    gamelog[i].exchange.cardtype3,
                    gamelog[i].exchange.troops);
        }
        else if(gamelog[i].type == LOG_TURN_END)
        {
            printf("Player %d ends turn.%s\n",
                    gamelog[i].turnend.player,
                    gamelog[i].turnend.roundEnd? " End of round." : "");
        }
    }

    for(int i = 0; i < numPlayers; i++)
    {
        int total = count[i][1] + count[i][2] + count[i][3] 
            + count[i][4] + count[i][5] + count[i][6];
        printf("Player %d summary\n-------\n", i);
        for(int n = 1; n <= 6; n++)
        {
            printf("%d: %6d %5.2f%%\n", n, count[i][n], 
                    count[i][n] / (double)total * 100.0);
        }
    }

}

void dumpStats()
{
    for(int i = 0; i < numPlayers; i++)
    {
        printf("-----\nPlayer %d:\n", i);
        printf("%d dice rolled\n", totalDiceRolls[i]);
        for(int j = 0; j < 6; j++)
            printf("    %d %ds\n", diceRolls[i][j], j+1);
        printf("%d reinforcements\n", reinforcementCount[i]);
        printf("%d kills\n", killCount[i]);
        printf("%d deaths\n", deathCount[i]);
        printf("\nLongest held: %d (%s), %d turns\n", 
                longestHeldTerritory[i].territory, 
                territories[longestHeldTerritory[i].territory].name,
                longestHeldTerritory[i].count);
    }

    printf("\n\nMost taken: %d (%s), %d times\n",
                mostTakenTerritory.territory, 
                territories[mostTakenTerritory.territory].name,
                mostTakenTerritory.count);
    printf("\nHolds\n");
    for(int i = 0; i < NUM_TERRITORIES; i++)
        printf("%d (%s): %d turns\n", i, territories[i].name, 
                territoryHoldTimes[i]);
    printf("Takens\n");
    for(int i = 0; i < NUM_TERRITORIES; i++)
        printf("%d (%s): %d times\n", i, territories[i].name,
                territoryTakenCounts[i]);
}

#define TRADE(a,b,c) cardInput(hands[currentPlayer].hand[a].index, hands[currentPlayer].hand[b].index, hands[currentPlayer].hand[c].index)
int main()
{
    int card1, card2;
    rng.seed(time(0));
    if(!font.loadFromFile("DroidSansMono.ttf"))
    {
        printf("can't load font\n");
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "RISK prototype");
    changeState(INIT);
    updateText();
    while(window.isOpen())
    {
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
                window.close();
            if(ev.type == sf::Event::KeyPressed)
            {
                if(ev.key.code == sf::Keyboard::Space)
                {
                    gameInput(ADVANCE);
                    card1 = card2 = -1;
                }
                if(ev.key.code == sf::Keyboard::LControl)
                {
                    gameInput(CANCEL);
                    card1 = card2 = -1;
                }
                if(ev.key.code == sf::Keyboard::Left)
                {
                    gameInput(PREVIOUS);
                    card1 = card2 = -1;
                }
                if(ev.key.code == sf::Keyboard::Right)
                {
                    gameInput(NEXT);
                    card1 = card2 = -1;
                }

                if(ev.key.code == sf::Keyboard::Num0)
                {
                    if(card1 == -1)
                        card1 = 0;
                    else if(card2 == -1 && card1 != 0)
                        card2 = 0;
                    else if(card2 != 0 && card1 != 0)
                    {
                        printf("traded for %d troops\n", 
                                TRADE(card1, card2, 0));
                        card1 = card2  = -1;
                    }
                }
                if(ev.key.code == sf::Keyboard::Num1)
                {
                    if(card1 == -1)
                        card1 = 1;
                    else if(card2 == -1 && card1 != 1)
                        card2 = 1;
                    else if(card2 != 1 && card1 != 1)
                    {
                        printf("traded for %d troops\n", 
                                TRADE(card1, card2, 1));
                        card1 = card2  = -1;
                    }
                }

                if(ev.key.code == sf::Keyboard::Num2)
                {
                    if(card1 == -1)
                        card1 = 2;
                    else if(card2 == -1 && card1 != 2)
                        card2 = 2;
                    else if(card2 != 2 && card1 != 2)
                    {
                        printf("traded for %d troops\n", 
                                TRADE(card1, card2, 2));
                        card1 = card2  = -1;
                    }
                }

                if(ev.key.code == sf::Keyboard::Num3)
                {
                    if(card1 == -1)
                        card1 = 3;
                    else if(card2 == -1 && card1 != 3)
                        card2 = 3;
                    else if(card2 != 3 && card1 != 3)
                    {
                        printf("traded for %d troops\n", 
                                TRADE(card1, card2, 3));
                        card1 = card2  = -1;
                    }
                }

                if(ev.key.code == sf::Keyboard::Num4)
                {
                    if(card1 == -1)
                        card1 = 4;
                    else if(card2 == -1 && card1 != 4)
                        card2 = 4;
                    else if(card2 != 4 && card1 != 4)
                    {
                        printf("traded for %d troops\n", 
                                TRADE(card1, card2, 4));
                        card1 = card2  = -1;
                    }
                }
                if(ev.key.code == sf::Keyboard::C)
                {
                    printf("Player %d cards: %d\n", currentPlayer, 
                            hands[currentPlayer].cards);
                    for(int i = 0; i < hands[currentPlayer].cards; i++)
                    {
                        printf("%d (%s, %d)\n", hands[currentPlayer].hand[i].type,
                                hands[currentPlayer].hand[i].type != WILD?  territories[hands[currentPlayer].hand[i].territory].name : "Wild",
                                hands[currentPlayer].hand[i].index);
                    }
                    printf("\n");
                }
                if(ev.key.code == sf::Keyboard::L)
                    dumpLog();
                if(ev.key.code == sf::Keyboard::P)
                    dumpStats();
                if(ev.key.code == sf::Keyboard::S)
                {
                    saveGame();
                    FILE * savefile = fopen("risk.save", "wb");
                    if(savefile)
                        fwrite(saveData, 1, 0x800, savefile);
                    fclose(savefile);
                }
                if(ev.key.code == sf::Keyboard::A)
                {
                    FILE * savefile = fopen("risk.save", "rb");
                    if(savefile)
                    {
                        fread(saveData, 1, 0x800, savefile);
                        fclose(savefile);
                        restoreGame();
                    }
                }
                
            }
        }
        window.clear();
        drawText(window);
        drawMap(window);
        drawDie(window, attackerDice[0], 300, 450);
        drawDie(window, attackerDice[1], 300, 490);
        drawDie(window, attackerDice[2], 300, 530);
        drawDie(window, defenderDice[0], 340, 450);
        drawDie(window, defenderDice[1], 340, 490);
        window.display(); 
        processStats();

        if(blinkclock.getElapsedTime() > sf::milliseconds(250))
            blinkclock.restart();
    }
    return 0;
}
