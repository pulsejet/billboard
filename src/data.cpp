#include <iostream>
#include <curl/curl.h>
#include "data.hpp"
#include "http.hpp"
using json = nlohmann::json;

/* ========== GLOBAL INITIALIZATION FUNCTIONS ========= */
void data_global_init() { curl_global_init(CURL_GLOBAL_DEFAULT); }
void data_global_clean() { curl_global_cleanup(); }
/* ==================================================== */

/** Inject dependency */
Data::Data(Config * config) {
    cfg = config;
}

void deleteFile(std::string path) {
    std::cout << "Deleting file " << path << std::endl;
    std::remove(path.c_str());
}

/** Get list of all events from API */
std::vector<Event> Data::getEvents() {
    std::vector<Event> eventVector;
    auto eventsString = requestStr(cfg, cfg->getS(K_EVENTS_URL));

    /* Check if our request failed */
    if (eventsString == NULL) {
        return eventVector;
    }

    /* Parse the json */
    json eventsJson = json::parse(*eventsString.get())["data"];

    /* Get all events */
    for (auto eventJson : eventsJson) {
        Event event(cfg, eventJson);

        /* Load image_url for the event */
        if (event.imageUrl != STRING_EMPTY) {
            if (file_exists(event.imageFileName) || requestImage(cfg, event.imageUrl)) {
                bool success = event.bigImage.loadFromFile(event.imageFileName);

                /* Check if the image loaded sucessfully */
                if (!success) {
                    deleteFile(event.imageFileName);
                    event.imageUrl = STRING_EMPTY;
                }
            } else {
                /* Set event image url to blank */
                event.imageUrl = STRING_EMPTY;
            }
        }

        /* Do NOT refactor this to an else condition, cause it isn't
         * one. If the image url is set to blank artificially,
         * this will cause the event to fall back to the body image */
        if (event.imageUrl == STRING_EMPTY && event.bodies[0].imageUrl != STRING_EMPTY) {
            /* Body constants to fallback to */
            Body body = event.bodies[0];
            const std::string filename = body.imageFileName;

            /* Load body image for fallback */
            if (file_exists(filename) || requestImage(cfg, body.imageUrl)) {
                bool success = event.bigImage.loadFromFile(filename);

                /* Check if the image loaded sucessfully */
                if (!success) {
                    deleteFile(filename);
                    body.imageUrl = STRING_EMPTY;
                }
            } else {
                /* Failed to find anything! */
                body.imageUrl = STRING_EMPTY;
            }
        }

        eventVector.push_back(event);
    }
    return eventVector;
}
