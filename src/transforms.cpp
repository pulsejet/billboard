#include "transforms.hpp"

#include "config.h"
#include "common.hpp"

/** Scale sprite to fill screen */
void scaleCenterSpriteFull(
    sf::Sprite& sprite,
    sf::Image& image,
    float maxHeight,
    bool fullHeight)
{
    float sx = image.getSize().x;
    float sy = image.getSize().y;

    float scaleFactorY = ((float) WINDOW_HEIGHT / sy);
    float scaleFactorX = ((float) WINDOW_WIDTH / sx);

    float scaleFactor = 1.0;

    if (scaleFactorY > scaleFactorX && !fullHeight) {
        scaleFactor = scaleFactorX;
    } else {
        scaleFactor = scaleFactorY * maxHeight;
    }

    float xTransform = ((float) WINDOW_WIDTH - sx * scaleFactor) / 2.0;
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(xTransform, 0);
}
