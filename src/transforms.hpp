#ifndef _INSTIAPP_TRANSFORMS_HPP
#define _INSTIAPP_TRANSFORMS_HPP

#include <SFML/Graphics.hpp>
#include "config.hpp"

/** Scale sprite to fill the whole screen with optional constraints */
void scaleCenterSpriteFull(
    Config * cfg,
    sf::Sprite& sprite,
    sf::Image& image,
    float maxHeight = 1.0f,
    bool fullHeight = false);

#endif
