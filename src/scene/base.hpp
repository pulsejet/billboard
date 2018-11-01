#ifndef _INSTIAPP_SCENE_BASE_HPP
#define _INSTIAPP_SCENE_BASE_HPP

#include <SFML/Graphics.hpp>

/** Base class for a single scene */
class Scene {
    public:

    /** Pass the whole window to the scene */
    virtual void create(sf::RenderWindow * window) = 0;

    /** Paint the window */
    virtual void paint() = 0;
};

#endif
