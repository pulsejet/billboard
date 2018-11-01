#ifndef _INSTIAPP_SCENE_EVENT_HPP
#define _INSTIAPP_SCENE_EVENT_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "base.hpp"
#include "../data.hpp"

class EventScene : public Scene {
    private:
    sf::RenderWindow * _window;

    /** Downloader */
    Data _data;

    /** Memory Cache of events */
    std::vector<Event> _events;

    /** Always maintain a timer */
    sf::Clock _clock;

    /** Current event being displayed */
    int _currentEventIndex = 0;

    /** Big image being shown */
    sf::Texture _currentBigTexture;
    sf::Sprite _currentBigSprite;
    sf::Texture _overlayGradientTexture;
    sf::Sprite _overlayGradient;

    /** Main texts */
    sf::Text _eventNameText;
    sf::Text _eventTimeText;

    sf::Font _font;

    void loadBigImage(Event event);

    public:
    /** Constructor */
    virtual void create(sf::RenderWindow * window);

    /** Paint the window */
    virtual void paint();
};

#endif
