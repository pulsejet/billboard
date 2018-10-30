#include "Painter.hpp"
#include <iostream>
#include "config.h"

void Painter::loadBigImage(Event event) {
    _currentBigTexture.loadFromImage(event.bigImage);
    _currentBigTexture.setSmooth(true);
    _currentBigSprite.setTexture(_currentBigTexture, true);
    float scaleFactor = (float) WINDOW_HEIGHT / (float) event.bigImage.getSize().y;
    float xTransform = ((float) WINDOW_WIDTH - (float) event.bigImage.getSize().x * scaleFactor) / 2.0;
    _currentBigSprite.setScale(scaleFactor, scaleFactor);
    _currentBigSprite.setPosition(xTransform, 0);
}

/** Constructor */
Painter::Painter(sf::RenderWindow * window) {
    _window = window;
    _events = _dataHandler.getEvents();

    while (_events[_currentEventIndex++].imageUrl == STRING_EMPTY);

    loadBigImage(_events[_currentEventIndex]);
}

/** Paint the window. Call this every iteration. */
void Painter::paint() {
    _window->draw(_currentBigSprite);

    if (_clock.getElapsedTime().asSeconds() > TIME_DELAY) {
        /* Reset the clock */
        _clock.restart();

        /* Load new image */
        while (_events[++_currentEventIndex].imageUrl == "");
        _currentEventIndex %= _events.size();
        loadBigImage(_events[_currentEventIndex]);
    }
}
