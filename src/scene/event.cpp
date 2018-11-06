#include <iostream>
#include <math.h>
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

    /* Define gradient vertex array */
    #define DEF_VERX(x, y, color) _overlayGrad.push_back(sf::Vertex(sf::Vector2f(x, y), color))
    DEF_VERX(0, cfg->getI(K_WINDOW_HEIGHT) * 0.55, sf::Color::Transparent);
    DEF_VERX(cfg->getI(K_WINDOW_WIDTH), cfg->getI(K_WINDOW_HEIGHT) * 0.55, sf::Color::Transparent);

    DEF_VERX(0, cfg->getI(K_WINDOW_HEIGHT) * 0.75, sf::Color::Black);
    DEF_VERX(cfg->getI(K_WINDOW_WIDTH), cfg->getI(K_WINDOW_HEIGHT) * 0.75, sf::Color::Black);

    DEF_VERX(0, cfg->getI(K_WINDOW_HEIGHT), sf::Color::Black);
    DEF_VERX(cfg->getI(K_WINDOW_WIDTH), cfg->getI(K_WINDOW_HEIGHT), sf::Color::Black);
    #undef DEF_VERX

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

    /* Setup logo stuff */
    _logoCircle.setFillColor(sf::Color::White);
    _logoCircle.setPointCount(100);
    _logoCircle.setPosition(0, 0);
    _logoTexture.loadFromMemory(&logo_png, logo_png_len);
    _logoTexture.setSmooth(true);
    _logoSprite.setTexture(_logoTexture);
    _logoWnccTexture.loadFromMemory(&logo_wncc_png, logo_wncc_png_len);
    _logoWnccTexture.setSmooth(true);
    _logoWnccSprite.setTexture(_logoWnccTexture);

    /* Logo texts */
    _logoName.setFont(_font);
    _logoName.setCharacterSize(cfg->getI(K_WINDOW_HEIGHT) / 20.0);
    _logoName.setString(cfg->getS(K_LOGO_NAME));

    _logoSuper.setFont(_font);
    _logoSuper.setCharacterSize(cfg->getI(K_WINDOW_HEIGHT) / 32.0);
    _logoSuper.setString(cfg->getS(K_LOGO_SUPER));

    /* Setup logo sprites position and scale */
    const int min_dim = std::min(cfg->getI(K_WINDOW_HEIGHT), cfg->getI(K_WINDOW_WIDTH));
    const float scale = LOGO_SCALE * cfg->getI(K_WINDOW_HEIGHT) / _logoSprite.getGlobalBounds().height;
    originCenter(&_logoSprite);
    centerScreen(cfg, &_logoSprite);
    _logoSprite.setScale(scale, scale);

    _logoWnccSprite.setOrigin(_logoWnccSprite.getGlobalBounds().width, _logoWnccSprite.getGlobalBounds().height);
    _logoWnccSprite.setScale(scale / 1.8f, scale / 1.8f);
    _logoWnccSprite.setPosition(cfg->getI(K_WINDOW_WIDTH) - min_dim * 0.04f, cfg->getI(K_WINDOW_HEIGHT) - min_dim * 0.04f);

    /* Logo text positions */
    originCenter(&_logoName);
    originCenter(&_logoSuper);
    centerScreen(cfg, &_logoName, 0, _logoSprite.getGlobalBounds().height / 1.7f);
    centerScreen(cfg, &_logoSuper, 0, -_logoSprite.getGlobalBounds().height / 1.7f);

    /* Initialize animation */
    if (cfg->getI(K_ANIMATION_ENABLED)) {
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

        /* Get the next event index to be displayed */
        if (++_currentEventIndex >= events.size()) _currentEventIndex = 0;
        while (!is_valid(cfg, events[_currentEventIndex])) {
            _currentEventIndex++;
            if (_currentEventIndex >= events.size()) _currentEventIndex = 0;
        };

        /* Load new image */
        loadBigImage(events[_currentEventIndex]);

        /* Reset the clock again
         * This is supposed to account for delays in copying textures
         * to video memory, leading to a skip at the start of animation */
        _clock.restart();
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

/* Show logo as necessary */
void EventScene::drawLogo() {
    const float t = _logoClock.getElapsedTime().asSeconds();
    const float T = cfg->getI(K_LOGO_SHOW_DURATION);
    const float v = cfg->getI(K_LOGO_ANIM_SPEED);
    const float ts = cfg->getI(K_LOGO_REPEAT_DELAY);
    const float fade_speed = cfg->getI(K_LOGO_FADE_SPEED);

    /* Show circle using linear interpolation */
    if (t > ts) {
        const float w = cfg->getI(K_WINDOW_WIDTH);
        const float r_th = ((T / 2) - std::abs((t - ts) - (T / 2))) * w * v;
        const float r_max = (T / 2) * w * v;
        const float r = std::min(r_th, w * 2);
        const float w_fade = w / 1.5;

        /* Draw circle */
        _logoCircle.setRadius(r);
        _logoCircle.setPosition(-r, -r);
        _window->draw(_logoCircle);

        /* Draw logo sprite */
        if (r_th > w_fade) {
            /* Get colors with alpha set for fade in */
            float alpha = std::min(((r_th - w_fade) / (r_max - w_fade)) * fade_speed, 1.0f);
            sf::Color whiteColor(255, 255, 255, alpha * 255);
            sf::Color blackColor(0, 0, 0, alpha * 255);

            /* Set paramters for sprites */
            _logoSprite.setColor(whiteColor);
            _logoWnccSprite.setColor(whiteColor);
            _logoName.setFillColor(blackColor);
            _logoSuper.setFillColor(blackColor);

            /* Draw */
            _window->draw(_logoSprite);
            _window->draw(_logoWnccSprite);
            _window->draw(_logoName);
            _window->draw(_logoSuper);
        }
    }

    /* Reset back to normal */
    if (t > ts + T) {
        _logoClock.restart();
        _logoCircle.setRadius(0);
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
    _window->draw(&_overlayGrad[0], _overlayGrad.size(), sf::TriangleStrip);
    _window->draw(_eventNameText);
    _window->draw(_eventTimeText);
    drawLogo();

    /* Show spinner while refreshing */
    if (refreshing) {
        _window->draw(_progressSprite);
        _progressSprite.setRotation(_refresh_clock.getElapsedTime().asSeconds() * 450);
    }
}

EventScene::~EventScene() {
    delete _bigSpriteAnim;

    if (_bgThread) {
        _bgThread->join();
        delete _bgThread;
    }
}
