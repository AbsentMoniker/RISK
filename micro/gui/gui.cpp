#include <SFML/Graphics.hpp>
extern "C" {
#include "../game.h"
#include "../io.h"
}

#include <cstdio>
#include <cstring>
#include <cstdarg>

char textdisplay[2][17] = {{'\0'},{'\0'}};
extern "C" {
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
}


sf::Vector2f terPositions[NUM_TERRITORIES] =
{
    {200, 200},
    {300, 200},
    {400, 200},
    {400, 300},
    {400, 400},
    {300, 400},
    {200, 400},
    {200, 300},
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
        sf::RectangleShape shape(sf::Vector2f(50, 50));
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
    line.setPosition(sf::Vector2f(20, 20));
    win.draw(line);

    line.setString(textdisplay[1]);
    line.setPosition(sf::Vector2f(20, 50));
    win.draw(line);

}

int main()
{
    if(!font.loadFromFile("DroidSansMono.ttf"))
    {
        printf("can't load font\n");
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "RISK prototype");
    initializeGame();
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
        window.display();

        if(blinkclock.getElapsedTime() > sf::milliseconds(250))
            blinkclock.restart();
    }
    return 0;
}
