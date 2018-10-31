#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Painter.hpp"
#include "config.h"

/** Scale sprite to fill screen */
void scaleCenterSpriteFull(sf::Sprite& sprite, sf::Image& image, float maxHeight = 1.0f) {
    float scaleFactor = ((float) WINDOW_HEIGHT / (float) image.getSize().y) * maxHeight;
    float xTransform = ((float) WINDOW_WIDTH - (float) image.getSize().x * scaleFactor) / 2.0;
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(xTransform, 0);
}

tm getTime(std::string dateStr) {
    int y,M,d,h,m;
    float s;
    int tzh = 0, tzm = 0;
    if (6 < sscanf(dateStr.c_str(), "%d-%d-%dT%d:%d:%f%d:%dZ", &y, &M, &d, &h, &m, &s, &tzh, &tzm)) {
        if (tzh < 0) {
        tzm = -tzm;    // Fix the sign on minutes.
        }
    }

    tm time;
    time.tm_year = y;
    time.tm_mon = M - 1;
    time.tm_mday = d;
    time.tm_hour = h;
    time.tm_min = m;
    time.tm_sec = (int) s;

    return time;
}

std::string monthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

std::string getSubtitle(Event event) {
    std::stringstream ss;

    tm startTime = getTime(event.startTime);

    std::string st[] = { "th", "st", "nd", "rd", "th", "th", "th", "th", "th", "th" };

    ss << std::setfill('0') << std::setw(2) << startTime.tm_hour <<  ":" << std::setfill('0') << std::setw(2) << startTime.tm_min;
    ss << " | ";
    ss << startTime.tm_mday << st[startTime.tm_mday % 10] << " " << monthNames[startTime.tm_mon];

    if (event.venueStr != STRING_EMPTY) {
        ss << " | " << event.venueStr;
    }

    return ss.str();
}

void Painter::loadBigImage(Event event) {
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
    _eventTimeText.setString(getSubtitle(event));
    _eventTimeText.setCharacterSize(height);
    _eventTimeText.setFillColor(sf::Color::White);
    _eventTimeText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 4.5);
}

/** Constructor */
Painter::Painter(sf::RenderWindow * window) {
    _window = window;
    _events = _dataHandler.getEvents();

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
        scaleCenterSpriteFull(_overlayGradient, overlayImage);
    }

    /* Skip events with no image */
    while (_events[_currentEventIndex++].imageUrl == STRING_EMPTY);

    loadBigImage(_events[_currentEventIndex]);
}

/** Paint the window. Call this every iteration. */
void Painter::paint() {
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
