#include <SFML/Graphics.hpp>
#include "painter.hpp"
#include "config.hpp"
#include "data.hpp"

int main()
{
    /* Load config */
    Config * cfg = new Config();

    /* Make window */
    sf::RenderWindow * window = new sf::RenderWindow(
        sf::VideoMode(cfg->getI(K_W), cfg->getI(K_H)),
        cfg->getS(K_WINDOW_TITLE),
        ((cfg->getB(K_FULLSCREEN)) ? sf::Style::Fullscreen : sf::Style::Default)
    );
    Painter painter(cfg, window);

    /* Hide mouse cursor */
    window->setMouseCursorVisible(false);

    /* Initialize data client */
    data_global_init();

    /* Prevent tearing on some devices */
    window->setVerticalSyncEnabled(true);

    /* Loop till we are wanted dead */
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
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
    delete cfg;
    delete window;
    return 0;
}
