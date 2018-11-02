#ifndef _INSTIAPP_PAINTER_HPP
#define _INSTIAPP_PAINTER_HPP

#include <SFML/Graphics.hpp>
#include "scene/base.hpp"

class Painter {
    private:
    sf::RenderWindow * _window;
    Config * cfg;
    Scene * scene;

    public:
    /** Constructor */
    Painter(Config * config, sf::RenderWindow * window);

    /** Clean up */
    ~Painter();

    /** Paint the window */
    void paint();
};

#endif
