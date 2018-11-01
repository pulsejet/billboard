#include <iostream>
#include "event.hpp"
#include "../transforms.hpp"

void EventScene::loadBigImage(Event event) {
    /* Load big image */
    _currentBigTexture.loadFromImage(event.bigImage);
    _currentBigTexture.setSmooth(true);
    _currentBigSprite.setTexture(_currentBigTexture, true);

    scaleCenterSpriteFull(_currentBigSprite, event.bigImage, 0.9f);

    /* Load texts */
    float height = WINDOW_HEIGHT / 18.0;
    _eventNameText.setFont(_font);
    _eventNameText.setString(event.name);
    _eventNameText.setCharacterSize(height);
    _eventNameText.setFillColor(sf::Color::White);
    _eventNameText.setStyle(sf::Text::Bold);
    _eventNameText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 5);

    height = WINDOW_HEIGHT / 22.0;
    _eventTimeText.setFont(_font);
    _eventTimeText.setString(event.getSubtitle());
    _eventTimeText.setCharacterSize(height);
    _eventTimeText.setFillColor(sf::Color::White);
    _eventTimeText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 4.5);
}

/** Constructor */
void EventScene::create(sf::RenderWindow * window) {
    _window = window;
    _events = _data.getEvents();

    /* Load font */
    if (!_font.loadFromFile(TITLE_FONT)) {
        std::cout << "Could not load font" << std::endl;
    }

    /* Load overlay gradient */
    sf::Image overlayImage;
    if (overlayImage.loadFromFile("fade.png")) {
        _overlayGradientTexture.loadFromImage(overlayImage);
        _overlayGradientTexture.setSmooth(true);
        _overlayGradient.setTexture(_overlayGradientTexture);
        scaleCenterSpriteFull(_overlayGradient, overlayImage, 1.0f, true);
    }

    /* Skip events with no image */
    while (_events[_currentEventIndex++].imageUrl == STRING_EMPTY);

    loadBigImage(_events[_currentEventIndex]);
}

/** Paint the window. Call this every iteration. */
void EventScene::paint() {
    _window->draw(_currentBigSprite);
    _window->draw(_overlayGradient);
    _window->draw(_eventNameText);
    _window->draw(_eventTimeText);

    if (_clock.getElapsedTime().asSeconds() > TIME_DELAY) {
        /* Reset the clock */
        _clock.restart();

        /* Load new image */
        if (++_currentEventIndex >= _events.size()) _currentEventIndex = 0;
        while (_events[_currentEventIndex].imageUrl == STRING_EMPTY ||
               _events[_currentEventIndex].weight < WEIGHT_THRESHOLD
        ) {
            _currentEventIndex++;
            if (_currentEventIndex >= _events.size()) _currentEventIndex = 0;
        };

        loadBigImage(_events[_currentEventIndex]);
    }
}
