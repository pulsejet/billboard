#include <SFML/Graphics.hpp>
#include "Painter.hpp"
#include "config.h"

int main()
{
    /* Make window */
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Noticeboard!");
    Painter painter(window);

    /* Initialize data client */
    data_global_init();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        /* Clear the window */
        window->clear();

        /* Draw Stuff */
        painter.paint();

        /* Display drawn stuf */
        window->display();
    }

    /* Clean up */
    data_global_clean();
    return 0;
}
