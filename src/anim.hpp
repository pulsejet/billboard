#ifndef _INSTIAPP_ANIM_HPP
#define _INSTIAPP_ANIM_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
    private:
    sf::Transformable * _sprite;

    int _baseX;
    int _baseY;

    bool has_lcr = false;
    int lcr_duration;
    int lcr_exponent;

    sf::Clock * _clock;

    public:
    Animation(sf::Transformable * sprite, sf::Clock * _clock);
    Animation(sf::Transformable * sprite, sf::Clock * _clock, int baseX, int baseY);

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

    public:
    AnimationGroup(std::vector<sf::Transformable*> sprites, sf::Clock * _clock);

    void rebase();
    void set_lcr(int duration, int speed);
    void animate();

    ~AnimationGroup();
};

#endif
