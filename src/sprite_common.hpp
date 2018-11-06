#ifndef _INSTIAPP_SPRITE_COMMON_HPP
#define _INSTIAPP_SPRITE_COMMON_HPP

#include <SFML/Graphics.hpp>
#include "assets.h"
#include "config.hpp"

/** Load sprite for progress spinner */
#define MAKE_SPRITE(asset, height_scale, center_origin) \
    sf::Sprite sprite; \
    if (texture->loadFromMemory(&asset, asset##_len)) {\
        texture->setSmooth(true);\
        sprite.setTexture(*texture);\
        const float scaling = (cfg->getI(K_H) / (float) sprite.getGlobalBounds().height) * (height_scale);\
        sprite.setScale(scaling, scaling);\
        if (center_origin) {\
            sprite.setOrigin(texture->getSize().x / 2.0, texture->getSize().y / 2.0);\
        }\
    }

inline sf::Sprite makeProgressSprite(Config * cfg, sf::Texture * texture) {
    MAKE_SPRITE(progress_png, 0.1, true)
    sprite.setPosition(cfg->getI(K_W) * 0.85, cfg->getI(K_H) * 0.85);
    return sprite;
}

inline sf::Sprite makeLogoSprite(Config * cfg, sf::Texture * texture) {
    MAKE_SPRITE(logo_png, LOGO_SCALE, true)
    return sprite;
}

inline sf::Sprite makeWnccLogoSprite(Config * cfg, sf::Texture * texture) {
    MAKE_SPRITE(logo_wncc_png, LOGO_SCALE / 3.8f, false);
    return sprite;
}

inline void makeText(
    Config * cfg,
    sf::Text * text,
    sf::Font * font,
    float height_frac,
    std::string str="",
    sf::Color fillColor=sf::Color::White
) {
    text->setFont(*font);
    text->setCharacterSize(cfg->getI(K_H) * height_frac);
    text->setFillColor(fillColor);
    if (str != "") {
        text->setString(str);
    }
}

#endif
