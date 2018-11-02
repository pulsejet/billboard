#ifndef _INSTIAPP_SPRITE_COMMON_HPP
#define _INSTIAPP_SPRITE_COMMON_HPP

#include <SFML/Graphics.hpp>
#include "assets.h"
#include "config.hpp"

/** Load sprite for progress spinner */
#define MAKE_SPRITE(asset, height_scale) \
    sf::Sprite sprite; \
    if (texture->loadFromMemory(&asset, asset##_len)) {\
        texture->setSmooth(true);\
        sprite.setTexture(*texture);\
        const float scaling = (cfg->getI(K_WINDOW_HEIGHT) / (float) texture->getSize().y) * height_scale;\
        sprite.setScale(scaling, scaling);\
        sprite.setOrigin(texture->getSize().x / 2.0, texture->getSize().y / 2.0);\
    }

inline sf::Sprite makeProgressSprite(Config * cfg, sf::Texture * texture) {
    MAKE_SPRITE(progress_png, 0.1)
    sprite.setPosition(cfg->getI(K_WINDOW_WIDTH) * 0.85, cfg->getI(K_WINDOW_HEIGHT) * 0.85);
    return sprite;
}

#endif
