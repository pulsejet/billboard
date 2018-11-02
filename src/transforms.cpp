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
