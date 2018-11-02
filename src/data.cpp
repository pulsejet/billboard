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
    auto eventsString = requestStr(cfg->getS(K_EVENTS_URL));

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
                event.imageUrl = STRING_EMPTY;
            }
        }

        eventVector.push_back(event);
    }
    return eventVector;
}
