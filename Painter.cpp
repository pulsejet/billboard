#include "Painter.hpp"
#include <iostream>
#include "config.h"

/** Load texture from path */
sf::Texture * getImageTexture(std::string path) {
    sf::Texture * texture = new sf::Texture();
    if (!texture->loadFromFile(path))
    {
        delete texture;
        return NULL;
    }
    texture->setSmooth(true);
    return texture;
}

/** Get main sprite for one image */
sf::Sprite * getSpirte(std::string path) {
    sf::Texture * texture = getImageTexture(path);
    if (texture != NULL) {
        sf::Sprite * sprite = new sf::Sprite();
        sprite->setTexture(*texture);
        return sprite;
    } else {
        return NULL;
    }
}

void Painter::loadBigImage(Event event) {
    if (_currentBigSprite && _currentBigTexture) {
        delete _currentBigSprite;
        delete _currentBigTexture;
    }
    _currentBigSprite = getSpirte(event.imageFileName);
}

/** Constructor */
Painter::Painter(sf::RenderWindow * window) {
    _window = window;
    _events = _dataHandler.getEvents();

    while (_events[_currentEventIndex++].imageUrl == "");

    loadBigImage(_events[_currentEventIndex]);
}

/** Paint the window. Call this every iteration. */
void Painter::paint() {
    _window->draw(*_currentBigSprite);

    if (_clock.getElapsedTime().asSeconds() > TIME_DELAY) {
        /* Reset the clock */
        _clock.restart();

        /* Load new image */
        while (_events[++_currentEventIndex].imageUrl == "");
        _currentEventIndex %= _events.size();
        loadBigImage(_events[_currentEventIndex]);
    }
}
