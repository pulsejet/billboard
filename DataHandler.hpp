
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "config.h"

void data_global_init();
void data_global_clean();

inline std::string imageFilename(std::string url) {
    return IMAGES_DIR + url.substr(url.find_last_of("/") + 1);
}

struct Body {
    std::string id;
    std::string strId;
    std::string name;
    std::string short_description;
    std::string imageUrl;

    Body(nlohmann::json json) {
        id = json["id"];
        strId = json["str_id"];
        name = json["name"];
        short_description = json["short_description"];

        // Image may be null
        if (json["image_url"] != nullptr) {
            imageUrl = json["image_url"];
        } else {
            imageUrl = STRING_EMPTY;
        }
    }
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

    Event(nlohmann::json json) {
        id = json["id"];
        strId = json["str_id"];
        name = json["name"];
        description = json["description"];
        startTime = json["start_time"];
        endTime = json["end_time"];
        weight = json["weight"];

        // Get bodies
        for (auto body : json["bodies"]) {
            bodies.push_back(Body(body));
        }

        // Image may be null
        if (json["image_url"] != nullptr) {
            imageUrl = json["image_url"];
        } else {
            imageUrl = bodies[0].imageUrl;
        }

        // Save filename
        if (imageUrl != STRING_EMPTY) {
            imageFileName = imageFilename(imageUrl);
        }

        // Calculate venue string
        for (auto x : json["venues"]) {
            if (venueStr != STRING_EMPTY) {
                venueStr += ", ";
            }
            venueStr += x["short_name"];
        }
    }
};

class DataHandler {
    public:
    std::vector<Event> getEvents();
    DataHandler();
    ~DataHandler();
};
