#include "Painter.hpp"
#include <iostream>
#include "config.h"

void Painter::loadBigImage(Event event) {
    /* Load big image */
    _currentBigTexture.loadFromImage(event.bigImage);
    _currentBigTexture.setSmooth(true);
    _currentBigSprite.setTexture(_currentBigTexture, true);
    float scaleFactor = (float) WINDOW_HEIGHT / (float) event.bigImage.getSize().y;
    float xTransform = ((float) WINDOW_WIDTH - (float) event.bigImage.getSize().x * scaleFactor) / 2.0;
    _currentBigSprite.setScale(scaleFactor, scaleFactor);
    _currentBigSprite.setPosition(xTransform, 0);

    /* Load texts */
    const float height = WINDOW_HEIGHT / 18.0;
    _eventNameText.setFont(_font);
    _eventNameText.setString(event.name);
    _eventNameText.setCharacterSize(height);
    _eventNameText.setFillColor(sf::Color::White);
    _eventNameText.setStyle(sf::Text::Bold);
    _eventNameText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 5);
}

/** Constructor */
Painter::Painter(sf::RenderWindow * window) {
    _window = window;
    _events = _dataHandler.getEvents();

    /* Load font */
    if (!_font.loadFromFile(TITLE_FONT)) {
        std::cout << "Could not load font" << std::endl;
    }

    /* Skip events with no image */
    while (_events[_currentEventIndex++].imageUrl == STRING_EMPTY);

    loadBigImage(_events[_currentEventIndex]);
}

/** Paint the window. Call this every iteration. */
void Painter::paint() {
    _window->draw(_currentBigSprite);
    _window->draw(_eventNameText);

    if (_clock.getElapsedTime().asSeconds() > TIME_DELAY) {
        /* Reset the clock */
        _clock.restart();

        /* Load new image */
        while (_events[++_currentEventIndex].imageUrl == "");
        _currentEventIndex %= _events.size();
        loadBigImage(_events[_currentEventIndex]);
    }
}
