#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

#include "DataHandler.hpp"

class Painter {
    private:
    sf::RenderWindow * _window;

    /** Downloader */
    DataHandler _dataHandler;

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
    Painter(sf::RenderWindow * window);

    /** Paint the window */
    void paint();
};
