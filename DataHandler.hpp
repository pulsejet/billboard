
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>

void data_global_init();
void data_global_clean();

inline std::string imageFilename(std::string url) {
    return "images/" + url.substr(url.find_last_of("/") + 1);
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
            imageUrl = "";
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
    std::vector<Body> bodies;

    Event(nlohmann::json json) {
        id = json["id"];
        strId = json["str_id"];
        name = json["name"];
        description = json["description"];
        startTime = json["start_time"];
        endTime = json["end_time"];

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
        if (imageUrl != "") {
            imageFileName = imageFilename(imageUrl);
        }
    }
};

class DataHandler {
    public:
    std::vector<Event> getEvents();
    DataHandler();
    ~DataHandler();
};
