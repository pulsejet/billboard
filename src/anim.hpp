#ifndef _INSTIAPP_ANIM_HPP
#define _INSTIAPP_ANIM_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "config.hpp"

class Animation {
    private:
    sf::Transformable * _sprite;

    /* Initial/final coordinates */
    int _baseX;
    int _baseY;

    /* Left-center-right */
    bool has_lcr = false;
    int lcr_duration;
    int lcr_exponent;

    /* Spinning */
    bool has_spin = false;
    int spin_deg;

    /* Animation Clock */
    sf::Clock * _clock;

    Config * cfg;

    public:
    Animation(Config * config, sf::Transformable * sprite, sf::Clock * _clock);
    Animation(Config * config, sf::Transformable * sprite, sf::Clock * _clock, int baseX, int baseY);

    /** Set base coordinates to current */
    void rebase();

    /** Animate sprite from left to center to right with a polynomial */
    void set_lcr(
        int duration,
        int speed
    );

    /** Spin sprite continuously about it's current origin */
    void set_spin(int rpm);

    /** Carry out all animations */
    void animate();
};

/** Helper to animate many things at once
 * WARNING: THIS DOES NOT MAKE ANIMATIONS ANY FASTER
 * This severely needs refactor to avoid repeated computations
 * while animation muliple objects. Currently, it just creates
 * an Animation object for each animation and animates everything
 * when the animate() method is called.
 */
class AnimationGroup {
    private:
    std::vector<Animation*> animations;
    Config * cfg;

    public:
    AnimationGroup(Config * config, std::vector<sf::Transformable*> sprites, sf::Clock * _clock);

    void rebase();
    void set_lcr(int duration, int speed);
    void animate();

    ~AnimationGroup();
};

#endif
