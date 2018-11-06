#include "anim.hpp"

#include <math.h>
#include "config.hpp"

#include <iostream>

Animation::Animation(Config * config, sf::Transformable * sprite, sf::Clock * clock, int baseX, int baseY) {
    cfg = config;
    _sprite = sprite;
    _clock = clock;
    _baseX = baseX;
    _baseY = baseY;
}

Animation::Animation(Config * config, sf::Transformable * sprite, sf::Clock * clock) : Animation(config, sprite, clock, 0, 0) {};

void Animation::rebase() {
    _baseX = (int) _sprite->getPosition().x;
    _baseY = (int) _sprite->getPosition().y;
}

void Animation::set_lcr(int duration, int speed) {
    has_lcr = true;
    lcr_duration = duration;
    lcr_exponent = 2 * speed - 1;
}

void Animation::set_spin(int rpm) {
    has_spin = true;
    spin_deg = ((float) rpm) * 6.0;
}

void Animation::animate() {
    if (!_clock || !_sprite) return;

    int finalX = _baseX;
    int finalY = _baseY;

    /* Left-center-right */
    if (has_lcr) {
        const float x = ((float) (_clock->getElapsedTime().asMilliseconds() % lcr_duration)) / ((float) lcr_duration);
        finalX += cfg->getI(K_W) * pow(0.5, lcr_exponent) * pow(4 * (x - 0.5), lcr_exponent);
    }

    /* Spin */
    if (has_spin) {
        _sprite->setRotation(_clock->getElapsedTime().asSeconds() * spin_deg);
    }

    /* Update position for specific animations */
    if (has_lcr) {
        _sprite->setPosition(finalX, finalY);
    }
}

AnimationGroup::AnimationGroup(Config * config, std::vector<sf::Transformable*> sprites, sf::Clock * _clock) {
    cfg = config;
    for (auto sprite : sprites) {
        animations.push_back(new Animation(cfg, sprite, _clock));
    }
}

#define LOOP_A for (auto a : animations)
void AnimationGroup::rebase() { LOOP_A a->rebase(); }
void AnimationGroup::set_lcr(int duration, int speed) { LOOP_A a->set_lcr(duration, speed); }
void AnimationGroup::animate() { LOOP_A a->animate(); }
AnimationGroup::~AnimationGroup() { LOOP_A if (a) delete a; }
#undef LOOP_A
