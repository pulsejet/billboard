#include <iostream>
#include "event.hpp"
#include "../transforms.hpp"
#include "../common.hpp"
#include "../sprite_common.hpp"
#include "../assets.h"
#include "../anim.hpp"

void EventScene::loadBigImage(Event event) {
    /* Load big image */
    _currentBigTexture.loadFromImage(event.bigImage);
    _currentBigTexture.setSmooth(true);
    _currentBigSprite.setTexture(_currentBigTexture, true);

    /* Transform sprites */
    scaleCenterSpriteFull(cfg, _currentBigSprite, event.bigImage, 0.9f);

    /* Set texts */
    _eventNameText.setString(event.name);
    _eventTimeText.setString(event.getSubtitle());

    if (cfg->getI(K_ANIMATION_ENABLED)) {
        /* Set base coordinates for animation */
        if (_bigSpriteAnim) _bigSpriteAnim->rebase();
    }
}

/** Refresh events from network */
void refreshEvents(const EventScene * scene) {
    Config config;
    Data data(&config);
    auto loaded = data.getEvents();
    print_time();
    scene->refreshing = false;

    /* Check failed calls */
    if (loaded.size() == 0) {
        std::cout << "No events found at network" << std::endl;
        return;
    }

    /* Reset the clock to prevent this happening forever */
    scene->_refresh_clock.restart();

    /* Lock and update events */
    std::lock_guard<std::mutex> guard(scene->events_mutex);
    scene->events = loaded;
    std::cout << "Loaded " << loaded.size() << " events from network" << std::endl;
}

/** Constructor */
void EventScene::create(Config * config, sf::RenderWindow * window) {
    cfg = config;
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
        scaleCenterSpriteFull(cfg, _overlayGradient, overlayImage, 1.0f, true);
    }

    /* Initialize texts */
    float height = cfg->getI(K_WINDOW_HEIGHT) / 18.0;
    _eventNameText.setFont(_font);

    _eventNameText.setCharacterSize(height);
    _eventNameText.setFillColor(sf::Color::White);
    _eventNameText.setStyle(sf::Text::Bold);
    _eventNameText.setPosition(cfg->getI(K_WINDOW_WIDTH) / 18.0, cfg->getI(K_WINDOW_HEIGHT) - height * 5);

    height = cfg->getI(K_WINDOW_HEIGHT) / 22.0;
    _eventTimeText.setFont(_font);
    _eventTimeText.setCharacterSize(height);
    _eventTimeText.setFillColor(sf::Color::White);
    _eventTimeText.setPosition(cfg->getI(K_WINDOW_WIDTH) / 18.0, cfg->getI(K_WINDOW_HEIGHT) - height * 4.5);

    /* Load spinner */
    _progressSprite = makeProgressSprite(cfg, &_progressTexture);

    if (cfg->getI(K_ANIMATION_ENABLED)) {
        /* Initialize animation */
        _bigSpriteAnim = new Animation(cfg, &_currentBigSprite, &_clock);
        _bigSpriteAnim->set_lcr(cfg->getI(K_TIME_DELAY) * 1000, cfg->getI(K_EVENT_ANIMATION_SPEED));
    }
}

/** Return if the event is valid to display on big screen */
bool is_valid(Config * cfg, Event& event) {
    return event.imageUrl != STRING_EMPTY &&
           event.weight > cfg->getI(K_WEIGHT_THRESHOLD);
}

/** Chore to change main image after each cycle */
void EventScene::choreBigImage() {
    /* Mark initialized and sync clocks */
    if (!_initialized) {
        if (refreshing) { return; }
        _initialized = true;
        _refresh_clock.restart();
    }

    /* Reset the clock */
    _clock.restart();

    /* Lock events */
    std::lock_guard<std::mutex> guard(events_mutex);
    if (events.size() > 0) {

        /* Check that we have at least one valid event */
        bool has_valid = false;
        for (Event event : events) {
            has_valid = has_valid || is_valid(cfg, event);
        }
        if (!has_valid) { return; }

        /* Load new image */
        if (++_currentEventIndex >= events.size()) _currentEventIndex = 0;
        while (!is_valid(cfg, events[_currentEventIndex])) {
            _currentEventIndex++;
            if (_currentEventIndex >= events.size()) _currentEventIndex = 0;
        };

        loadBigImage(events[_currentEventIndex]);
    }
}

void EventScene::choreRefresh() {
    /* Check for existing threads */
    if (_bgThread) {
        _bgThread->join();
        delete _bgThread;
        _bgThread = nullptr;
    }

    /* Check on the condition again just to be sure */
    if (_refresh_clock.getElapsedTime().asSeconds() > cfg->getI(K_REFRESH_DURATION)) {
        refreshing = true;
        _bgThread = new std::thread(refreshEvents, this);
        _refresh_clock.restart();
    }
}

/** Paint the window. Call this every iteration. */
void EventScene::paint() {

    /* Update big image and texts */
    if (_clock.getElapsedTime().asSeconds() > cfg->getI(K_TIME_DELAY) || !_initialized) {
        choreBigImage();
    }

    /* Refresh events */
    if (_refresh_clock.getElapsedTime().asSeconds() > cfg->getI(K_REFRESH_DURATION)) {
        choreRefresh();
    }

    if (cfg->getI(K_ANIMATION_ENABLED)) {
        /* Animate sprite before drawing */
        _bigSpriteAnim->animate();
    }

    /* Draw everything */
    _window->draw(_currentBigSprite);
    _window->draw(_overlayGradient);
    _window->draw(_eventNameText);
    _window->draw(_eventTimeText);

    /* Show spinner while refreshing */
    if (refreshing) {
        _window->draw(_progressSprite);
        _progressSprite.setRotation(_clock.getElapsedTime().asSeconds() * 450);
    }
}

EventScene::~EventScene() {
    delete _bigSpriteAnim;

    if (_bgThread) {
        _bgThread->join();
        delete _bgThread;
    }
}
