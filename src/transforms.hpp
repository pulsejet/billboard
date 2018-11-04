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

/** Set origin to center */
void originCenter(sf::Sprite * obj);
void originCenter(sf::Text * obj);

/** Set position to center screen (requires origin at center) */
void centerScreen(Config * cfg, sf::Sprite * obj);
void centerScreen(Config * cfg, sf::Text * obj);
void centerScreen(Config * cfg, sf::Sprite * obj, float x, float y);
void centerScreen(Config * cfg, sf::Text * obj, float x, float y);

#endif
