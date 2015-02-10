#include <SFML/Graphics.hpp>
extern "C" {
#include "../game.h"
#include "../gamedata.h"
#include "../types.h"
#include "../territory.h"
#include "../io.h"
#include "../cards.h"
#include "../log.h"
}

#include <cstdio>
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
}


sf::Vector2f terPositions[NUM_TERRITORIES] =
{
    {10,10}, // T_ALASKA 0
    {60,10}, // T_NORTHWEST 1
    {140,10}, // T_GREENLAND 2
    {60,60}, // T_ALBERTA 3
    {110,60}, // T_ONTARIO 4
    {160,60}, // T_QUEBEC 5
    {70,110}, // T_WESTERN_US 6
    {120,110}, // T_EASTERN_US 7
    {90,160}, // T_MEXICO 8
    {120,250}, // T_VENEZUELA 9
    {100,300}, // T_PERU 10
    {120,350}, // T_ARGENTINA 11
    {150,300}, // T_BRAZIL 12
    {250,10}, // T_ICELAND 13
    {320,10}, // T_SCANDINAVIA 14
    {270,60}, // T_GREAT_BRITAIN 15
    {320,60}, // T_NORTHERN_EUROPE 16
    {280,120}, // T_WESTERN_EUROPE 17
    {340,120}, // T_SOUTHERN_EUROPE 18
    {380,70}, // T_UKRAINE 19
    {280,220}, // T_NORTHERN_AFRICA 20
    {330,210}, // T_EGYPT 21
    {330,260}, // T_CONGO 22
    {380,250}, // T_EASTERN_AFRICA 23
    {330,310}, // T_SOUTH_AFRICA 24
    {380,310}, // T_MADAGASCAR 25
    {440,50}, // T_URALS 26
    {490,50}, // T_SIBERIA 27
    {540,20}, // T_YAKUTSK 28
    {600,20}, // T_KAMCHATKA 29
    {540,70}, // T_IRKUSTK 30
    {470,130}, // T_AFGHANISTAN 31
    {620,130}, // T_JAPAN 32
    {560,130}, // T_MONGOLIA 33
    {460,180}, // T_MIDDLE_EAST 34
    {560,180}, // T_CHINA 35
    {540,230}, // T_INDIA 36
    {590,230}, // T_SIAM 37
    {580,340}, // T_INDONESIA 38
    {640,340}, // T_NEW_GUINEA 39
    {590,400}, // T_WESTERN_AUSTRAILIA 40
    {640,400}, // T_EASTERN_AUSTRAILIA 41
};
sf::Color playerColors[MAX_PLAYERS] =
{
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Cyan,
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
                                cardInput(card1, card2, 0));
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
                                cardInput(card1, card2, 1));
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
                                cardInput(card1, card2, 2));
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
                                cardInput(card1, card2, 3));
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
                                cardInput(card1, card2, 4));
                        card1 = card2  = -1;
                    }
                }
                if(ev.key.code == sf::Keyboard::C)
                {
                    printf("Player %d cards: %d\n", currentPlayer, 
                            hands[currentPlayer].cards);
                    for(int i = 0; i < hands[currentPlayer].cards; i++)
                    {
                        printf("%d (%s)\n", hands[currentPlayer].hand[i].type,
                                hands[currentPlayer].hand[i].type != WILD?  territories[hands[currentPlayer].hand[i].territory].name : "Wild");
                    }
                    printf("\n");
                }
                if(ev.key.code == sf::Keyboard::L)
                {
                    int count[numPlayers][7] = {0};
                    for(int i = 0; i < gamelogSize; i++)
                    {
                        if(gamelog[i].type == LOG_BATTLE)
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
                        else if(gamelog[i].type == LOG_REINFORCE)
                        {
                            printf("Player %d: %d troops placed in %s\n",
                                    gamelog[i].reinforce.player,
                                    gamelog[i].reinforce.troops,
                                    territories[
                                        gamelog[i].reinforce.territory].name);
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

        if(blinkclock.getElapsedTime() > sf::milliseconds(250))
            blinkclock.restart();
    }
    return 0;
}
