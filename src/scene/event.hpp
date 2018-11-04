#ifndef _INSTIAPP_SCENE_EVENT_HPP
#define _INSTIAPP_SCENE_EVENT_HPP

#include <thread>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <vector>
#include "base.hpp"
#include "../data.hpp"
#include "../anim.hpp"

class EventScene : public Scene {
    private:
    Config * cfg;
    sf::RenderWindow * _window;

    bool _initialized = false;

    /** Timers for multiple things */
    sf::Clock _clock;
    sf::Clock _logoClock;

    /** Current event being displayed */
    size_t _currentEventIndex = 0;

    /** Big image being shown */
    sf::Texture _currentBigTexture;
    sf::Sprite _currentBigSprite;
    Animation * _bigSpriteAnim;

    std::vector<sf::Vertex> _overlayGrad;

    /** Other sprites */
    sf::Texture _progressTexture;
    sf::Sprite _progressSprite;
    sf::CircleShape _logoCircle;
    sf::Texture _logoTexture;
    sf::Sprite _logoSprite;

    /** Main texts */
    sf::Text _eventNameText;
    sf::Text _eventTimeText;

    sf::Font _font;

    /** Data thread */
    std::thread * _bgThread;

    void loadBigImage(Event event);
    void choreBigImage();
    void choreRefresh();
    void drawLogo();

    public:
    /** Constructor */
    virtual void create(Config * config, sf::RenderWindow * window);

    /** Paint the window */
    virtual void paint();

    /** Set events to new list */
    void setEvents(std::vector<Event> events);

    /** Memory Cache of events */
    mutable std::vector<Event> events;
    mutable std::mutex events_mutex;
    mutable bool refreshing = true;
    mutable sf::Clock _refresh_clock;

    /** Destructor */
    ~EventScene();
};

#endif
