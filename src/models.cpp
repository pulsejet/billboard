#include <sstream>
#include <iomanip>
#include "models.hpp"
#include "config.hpp"
#include "common.hpp"

Body::Body(Config * cfg, nlohmann::json json) {
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

Event::Event(Config * cfg, nlohmann::json json) {
    id = json["id"];
    strId = json["str_id"];
    name = json["name"];
    description = json["description"];
    startTime = json["start_time"];
    endTime = json["end_time"];
    weight = json["weight"];

    // Get bodies
    for (auto body : json["bodies"]) {
        bodies.push_back(Body(cfg, body));
    }

    // Image may be null
    if (json["image_url"] != nullptr) {
        imageUrl = json["image_url"];
    } else {
        imageUrl = bodies[0].imageUrl;
    }

    // Save filename
    if (imageUrl != STRING_EMPTY) {
        imageFileName = imageFilename(cfg, imageUrl);
    }

    // Calculate venue string
    for (auto x : json["venues"]) {
        if (venueStr != STRING_EMPTY) {
            venueStr += ", ";
        }
        venueStr += x["short_name"];
    }
}

std::string Event::getSubtitle() {
    std::stringstream ss;

    tm tmStartTime = getTime(startTime);

    ss << std::setfill('0') << std::setw(2) << tmStartTime.tm_hour <<  ":" << std::setfill('0') << std::setw(2) << tmStartTime.tm_min;
    ss << " | ";
    ss << tmStartTime.tm_mday << dateSuffixes[tmStartTime.tm_mday % 10] << " " << monthNames[tmStartTime.tm_mon];

    if (venueStr != STRING_EMPTY) {
        ss << " | " << venueStr;
    }

    return ss.str();
}
