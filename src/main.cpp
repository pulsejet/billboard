#include <SFML/Graphics.hpp>
#include "painter.hpp"
#include "config.h"
#include "data.hpp"

int main()
{
    /* Make window */
    sf::RenderWindow * window = new sf::RenderWindow(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        WINDOW_TITLE,
        ((FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Default)
    );
    Painter painter(window);

    /* Initialize data client */
    data_global_init();

    //window->setFramerateLimit(30);
    window->setVerticalSyncEnabled(true);

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

        /* Sleep */
        window->display();
    }

    /* Clean up */
    data_global_clean();
    return 0;
}
