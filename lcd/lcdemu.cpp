#include <SFML/Graphics.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
    extern int characters[128][35];
}

bool filenameSelected = false;
sf::String filename;
sf::String lcdtext;

#define PX 3
#define SP 3
#define WIDTH (20*(PX*5+SP) + 50)
#define HEIGHT (4*(PX*7+SP*2) + 30)

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "LCD!");

    lcdtext = "                                                                                ";
    int lcdtextpos = 0;


    while(window.isOpen())
    {
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
                window.close();
            if(ev.type == sf::Event::TextEntered)
            {
                if(!filenameSelected)
                {
                    int ch = ev.text.unicode;
                    if(ch >= 32 && ch < 128)
                    {
                        lcdtext[lcdtextpos] = ch;
                        lcdtextpos += 1;
                        if(lcdtextpos >= 80)
                            lcdtextpos = 0;
                    }
                    if(ch == 8)
                    {
                        lcdtextpos -= 1;
                        if(lcdtextpos < 0)
                            lcdtextpos = 79;
                        lcdtext[lcdtextpos] = ' ';
                    }
                    if(ch == 1)
                    {
                        lcdtext[lcdtextpos] = 1;
                        lcdtextpos += 1;
                        if(lcdtextpos >= 80)
                            lcdtextpos = 0;
                    }
                }
                else
                {
                    printf("%c", ev.text.unicode);
                    fflush(stdout);
                    if(ev.text.unicode == 13)
                    {
                        printf("\n");
                        if(filename.getSize() > 0)
                        {
                            filename += ".png";
                            sf::Image img = window.capture();
                            img.saveToFile(filename);
                            printf("Saved!\n");
                        }
                        filename = "";
                    }
                    else if(ev.text.unicode == 8)
                    {
                        filename.erase(filename.getSize() - 1, 1);
                    }
                    else
                        filename += ev.text.unicode;
                }
            }
            if(ev.type == sf::Event::MouseButtonPressed)
            {
                if(ev.mouseButton.button == sf::Mouse::Right)
                    filenameSelected = !filenameSelected;
            }
        }
        window.clear();

        sf::RectangleShape disp(sf::Vector2f(WIDTH - 10, HEIGHT - 10));
        disp.setFillColor(sf::Color(0x88, 0xFF, 0x00));
        disp.setPosition(sf::Vector2f(5,5));
        window.draw(disp);

        sf::RectangleShape dot(sf::Vector2f(PX-2,PX-2));
        dot.setFillColor(sf::Color(0,0,0));
        dot.setOutlineColor(sf::Color(0x18, 0x30, 0x00));
        dot.setOutlineThickness(1);

        for(int i = 0; i < 80; i++)
        {
            for(int x = 0; x < 5; x++)
            {
                for(int y = 0; y < 7; y++)
                {
                    char ch = lcdtext[i];
                    if(characters[(int)ch][y*5 + x])
                    {
                        dot.setPosition(sf::Vector2f(
                                25 + (i % 20)*(5*(PX)+SP) + x*(PX),
                                15 + (i / 20)*(7*(PX)+2*SP) + y*(PX)));
                        window.draw(dot);
                    }
                }
            }
        }

        window.display();
    }
    return 0;
}
