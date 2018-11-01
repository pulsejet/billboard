#ifndef _INSTIAPP_MODELS_HPP
#define _INSTIAPP_MODELS_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "common.hpp"

struct Body {
    std::string id;
    std::string strId;
    std::string name;
    std::string short_description;
    std::string imageUrl;

    Body(nlohmann::json json);
};

struct Event {
    std::string id;
    std::string strId;
    std::string name;
    std::string description;
    std::string imageUrl;
    std::string startTime;
    std::string endTime;
    std::string imageFileName;
    int weight;
    std::string venueStr = STRING_EMPTY;
    std::vector<Body> bodies;

    sf::Image bigImage;

    Event(nlohmann::json json);

    std::string getSubtitle();
};

#endif