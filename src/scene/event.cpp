#include <iostream>
#include "event.hpp"
#include "../transforms.hpp"
#include "../common.hpp"
#include "../sprite_common.hpp"
#include "../assets.h"

#if ANIMATION_ENABLED
#include "../anim.hpp"
#endif

void EventScene::loadBigImage(Event event) {
    /* Load big image */
    _currentBigTexture.loadFromImage(event.bigImage);
    _currentBigTexture.setSmooth(true);
    _currentBigSprite.setTexture(_currentBigTexture, true);

    /* Transform sprites */
    scaleCenterSpriteFull(_currentBigSprite, event.bigImage, 0.9f);

    /* Set texts */
    _eventNameText.setString(event.name);
    _eventTimeText.setString(event.getSubtitle());

#if ANIMATION_ENABLED
    /* Set base coordinates for animation */
    if (_bigSpriteAnim) _bigSpriteAnim->rebase();
#endif
}

/** Refresh events from network */
void refreshEvents(const EventScene * scene) {
    Data data;
    auto loaded = data.getEvents();
    std::lock_guard<std::mutex> guard(scene->events_mutex);
    scene->events = loaded;
    print_time();
    scene->refreshing = false;
    std::cout << "Loaded " << loaded.size() << " events from network" << std::endl;
}

/** Constructor */
void EventScene::create(sf::RenderWindow * window) {
    _window = window;

    /* Get the events */
    _bgThread = new std::thread(refreshEvents, this);

    /* Load font */
    if (!_font.loadFromMemory(&roboto_light_ttf, roboto_light_ttf_len)) {
        std::cout << "Could not load font" << std::endl;
    }

    /* Load overlay gradient */
    sf::Image overlayImage;
    if (overlayImage.loadFromMemory(&fade_png, fade_png_len)) {
        _overlayGradientTexture.loadFromImage(overlayImage);
        _overlayGradientTexture.setSmooth(true);
        _overlayGradient.setTexture(_overlayGradientTexture);
        scaleCenterSpriteFull(_overlayGradient, overlayImage, 1.0f, true);
    }

    /* Initialize texts */
    float height = WINDOW_HEIGHT / 18.0;
    _eventNameText.setFont(_font);

    _eventNameText.setCharacterSize(height);
    _eventNameText.setFillColor(sf::Color::White);
    _eventNameText.setStyle(sf::Text::Bold);
    _eventNameText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 5);

    height = WINDOW_HEIGHT / 22.0;
    _eventTimeText.setFont(_font);
    _eventTimeText.setCharacterSize(height);
    _eventTimeText.setFillColor(sf::Color::White);
    _eventTimeText.setPosition(WINDOW_WIDTH / 18.0, WINDOW_HEIGHT - height * 4.5);

    /* Load spinner */
    _progressSprite = makeProgressSprite(&_progressTexture);

#if ANIMATION_ENABLED
    /* Initialize animation */
    _bigSpriteAnim = new Animation(&_currentBigSprite, &_clock);
    _bigSpriteAnim->set_lcr(TIME_DELAY * 1000, EVENT_ANIMATION_SPEED);
#endif
}

/** Paint the window. Call this every iteration. */
void EventScene::paint() {

#if ANIMATION_ENABLED
    /* Animate sprite before drawing */
    _bigSpriteAnim->animate();
#endif

    /* Draw everything */
    _window->draw(_currentBigSprite);
    _window->draw(_overlayGradient);
    _window->draw(_eventNameText);
    _window->draw(_eventTimeText);

    /* Wait for initialization */
    if (events.size() == 0 || refreshing) {
        _window->draw(_progressSprite);
        _progressSprite.setRotation(_clock.getElapsedTime().asSeconds() * 450);
        return;
    }

    if (_clock.getElapsedTime().asSeconds() > TIME_DELAY || !_initialized) {
        /* Mark initialized and sync clocks */
        if (!_initialized) {
            _initialized = true;
            _refresh_clock.restart();
        }

        /* Reset the clock */
        _clock.restart();

        /* Lock events */
        std::lock_guard<std::mutex> guard(events_mutex);

        /* Load new image */
        if (++_currentEventIndex >= events.size()) _currentEventIndex = 0;
        while (events[_currentEventIndex].imageUrl == STRING_EMPTY ||
               events[_currentEventIndex].weight < WEIGHT_THRESHOLD
        ) {
            _currentEventIndex++;
            if (_currentEventIndex >= events.size()) _currentEventIndex = 0;
        };

        loadBigImage(events[_currentEventIndex]);
    }

    /* Refresh events */
    if (_refresh_clock.getElapsedTime().asSeconds() > REFRESH_DURATION) {
        _bgThread->join();
        delete _bgThread;
        refreshing = true;
        _bgThread = new std::thread(refreshEvents, this);
        _refresh_clock.restart();
    }
}

EventScene::~EventScene() {
    delete _bigSpriteAnim;

    if (_bgThread) {
        _bgThread->join();
        delete _bgThread;
    }
}
