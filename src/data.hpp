#ifndef _INSTIAPP_DATA_HPP
#define _INSTIAPP_DATA_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "models.hpp"

void data_global_init();
void data_global_clean();

class Data {
    private:
    Config * cfg;

    public:

    /** Inject config */
    Data(Config * config);

    /** Get list of all events from API */
    std::vector<Event> getEvents();
};

#endif
