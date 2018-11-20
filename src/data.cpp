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
                event.bigImage.loadFromFile(event.imageFileName);
            } else {
                /* Set event image url to blank */
                event.imageUrl = STRING_EMPTY;
            }
        }

        /* Do NOT refactor this to an else condition, cause it isn't
         * one. If the image url is set to blank artificially,
         * this will cause the event to fall back to the body image */
        if (event.imageUrl == STRING_EMPTY && event.bodies[0].imageUrl != STRING_EMPTY) {
            /* Load body image for fallback */
            if (file_exists(event.bodies[0].imageFileName) || requestImage(cfg, event.bodies[0].imageUrl)) {
                event.bigImage.loadFromFile(event.bodies[0].imageFileName);
            } else {
                /* Failed to find anything! */
                event.bodies[0].imageUrl = STRING_EMPTY;
            }
        }

        eventVector.push_back(event);
    }
    return eventVector;
}
