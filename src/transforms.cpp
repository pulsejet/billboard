#include "transforms.hpp"

#include "config.hpp"
#include "common.hpp"

/** Scale sprite to fill screen */
void scaleCenterSpriteFull(
    Config * cfg,
    sf::Sprite& sprite,
    sf::Image& image,
    float maxHeight,
    bool fullHeight)
{
    float sx = image.getSize().x;
    float sy = image.getSize().y;

    float scaleFactorY = ((float) cfg->getI(K_WINDOW_HEIGHT) / sy);
    float scaleFactorX = ((float) cfg->getI(K_WINDOW_WIDTH) / sx);

    float scaleFactor = 1.0;

    if (scaleFactorY > scaleFactorX && !fullHeight) {
        scaleFactor = scaleFactorX;
    } else {
        scaleFactor = scaleFactorY * maxHeight;
    }

    float xTransform = ((float) cfg->getI(K_WINDOW_WIDTH) - sx * scaleFactor) / 2.0;
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(xTransform, 0);
}

/** Set origin to center */
#define ORIGIN_CENTER {\
    const auto bounds = obj->getLocalBounds();\
    obj->setOrigin(bounds.width / 2, bounds.height / 2);\
}

void originCenter(sf::Sprite * obj) ORIGIN_CENTER;
void originCenter(sf::Text * obj) ORIGIN_CENTER;

#undef ORIGIN_CENTER

/** Set origin to right lower corner */
#define ORIGIN_RL {\
    const auto bounds = obj->getLocalBounds();\
    obj->setOrigin(bounds.width, bounds.height);\
}

void originRL(sf::Sprite * obj) ORIGIN_RL;
void originRL(sf::Text * obj) ORIGIN_RL;

#undef ORIGIN_RL

/** Set position to center screen (requires origin at center) */
#define CENTER_SCREEN {\
    obj->setPosition(cfg->getI(K_WINDOW_WIDTH) / 2.0 + x, cfg->getI(K_WINDOW_HEIGHT) / 2.0 + y);\
}

void centerScreen(Config * cfg, sf::Sprite * obj, float x, float y) CENTER_SCREEN;
void centerScreen(Config * cfg, sf::Text * obj, float x, float y) CENTER_SCREEN;

void centerScreen(Config * cfg, sf::Sprite * obj) { centerScreen(cfg, obj, 0, 0); }
void centerScreen(Config * cfg, sf::Text * obj) { centerScreen(cfg, obj, 0, 0); }

#undef CENTER_SCREEN
