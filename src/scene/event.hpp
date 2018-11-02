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
    sf::RenderWindow * _window;

    bool _initialized = false;

    /** Always maintain a timer */
    sf::Clock _clock;
    sf::Clock _refresh_clock;

    /** Current event being displayed */
    size_t _currentEventIndex = 0;

    /** Big image being shown */
    sf::Texture _currentBigTexture;
    sf::Sprite _currentBigSprite;
    Animation * _bigSpriteAnim;
    sf::Texture _overlayGradientTexture;
    sf::Sprite _overlayGradient;

    /** Other sprites */
    sf::Texture _progressTexture;
    sf::Sprite _progressSprite;

    /** Main texts */
    sf::Text _eventNameText;
    sf::Text _eventTimeText;

    sf::Font _font;

    /** Data thread */
    std::thread * _bgThread;

    void loadBigImage(Event event);
    void choreBigImage();
    void choreRefresh();

    public:
    /** Constructor */
    virtual void create(sf::RenderWindow * window);

    /** Paint the window */
    virtual void paint();

    /** Set events to new list */
    void setEvents(std::vector<Event> events);

    /** Memory Cache of events */
    mutable std::vector<Event> events;
    mutable std::mutex events_mutex;
    mutable bool refreshing = true;

    /** Destructor */
    ~EventScene();
};

#endif
