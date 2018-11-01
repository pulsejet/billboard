#ifndef _INSTIAPP_ANIM_HPP
#define _INSTIAPP_ANIM_HPP

#include <SFML/Graphics.hpp>

class Animation {
    private:
    sf::Sprite * _sprite;

    int _baseX;
    int _baseY;

    bool has_lcr = false;
    int lcr_duration;
    int lcr_exponent;

    sf::Clock * _clock;

    public:
    Animation(sf::Sprite * sprite, sf::Clock * _clock);
    Animation(sf::Sprite * sprite, sf::Clock * _clock, int baseX, int baseY);

    /** Set base coordinates to current */
    void rebase();

    /** Animate sprite from left to center to right with a polynomial */
    void set_lcr(
        int duration,
        int speed
    );

    /** Carry out all animations */
    void animate();
};

#endif
