#include <SFML/Graphics.hpp>
extern "C" {
#include "../game.h"
#include "../gamedata.h"
#include "../types.h"
#include "../territory.h"
#include "../io.h"
}

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <random>
#include <ctime>

std::mt19937 rng;

// Implement the io functions needed by io.h
extern "C" {
    char textdisplay[2][17] = {{'\0'},{'\0'}};
    void setTextDisplay(int line, const char * format, ...)
    {
        va_list args;
        va_start(args, format);
        char str[16];
        vsnprintf(str, 16, format, args);
        va_end(args);

        if(line == 0)
        {
            strncpy(textdisplay[0], "                ", 16);
            strncpy(textdisplay[0], str, 16);
        }
        else if(line == 1)
        {
            strncpy(textdisplay[1], "                ", 16);
            strncpy(textdisplay[1], str, 16);
        }
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
    line.setPosition(sf::Vector2f(20, 520));
    win.draw(line);

    line.setString(textdisplay[1]);
    line.setPosition(sf::Vector2f(20, 550));
    win.draw(line);
}

void drawDie(sf::RenderWindow & win, int d, int x, int y)
{
    if(d < 0)
        return;
    sf::Text num(std::to_string(d), font, 24);
    num.setColor(sf::Color::Red);
    num.setPosition(x,y);
    win.draw(num);
}

int main()
{
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
                    gameInput(ADVANCE);
                if(ev.key.code == sf::Keyboard::LControl)
                    gameInput(CANCEL);
                if(ev.key.code == sf::Keyboard::Left)
                    gameInput(PREVIOUS);
                if(ev.key.code == sf::Keyboard::Right)
                    gameInput(NEXT);
            }
        }
        window.clear();
        drawMap(window);
        drawText(window);
        drawDie(window, attackerDice[0], 200, 450);
        drawDie(window, attackerDice[1], 200, 490);
        drawDie(window, attackerDice[2], 200, 530);
        drawDie(window, defenderDice[0], 240, 450);
        drawDie(window, defenderDice[1], 240, 490);
        window.display();

        if(blinkclock.getElapsedTime() > sf::milliseconds(250))
            blinkclock.restart();
    }
    return 0;
}
