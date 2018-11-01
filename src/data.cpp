#include <iostream>
#include <curl/curl.h>
#include "data.hpp"
#include "http.hpp"
using json = nlohmann::json;

/* ========== GLOBAL INITIALIZATION FUNCTIONS ========= */
void data_global_init() { curl_global_init(CURL_GLOBAL_DEFAULT); }
void data_global_clean() { curl_global_cleanup(); }
/* ==================================================== */

/** Get list of all events from API */
std::vector<Event> Data::getEvents() {
    auto eventsString = requestStr(EVENTS_URL);
    json eventsJson = json::parse(*eventsString.get())["data"];

    std::vector<Event> eventVector;
    for (auto eventJson : eventsJson) {
        Event event(eventJson);

        /* Load image_url for the event */
        if (event.imageUrl != STRING_EMPTY) {
            if (file_exists(event.imageFileName) || requestImage(event.imageUrl)) {
                event.bigImage.loadFromFile(event.imageFileName);
            }
        }

        eventVector.push_back(event);
    }
    return eventVector;
}
