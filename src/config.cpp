#include "config.hpp"
#include "common.hpp"
#include "assets.h"
#include <fstream>
#include <SFML/Window.hpp>

Config::Config() {
    /* Load config if file exists */
    if (file_exists(CONFIG_PATH)) {
        std::ifstream ifs(CONFIG_PATH);
        conf = nlohmann::json::parse(ifs);
        ifs.close();
    }

    /* Load default config as an asset */
    default_conf = nlohmann::json::parse(config_json);
}

/* Check for fallbacks */
nlohmann::json check(std::string prop, nlohmann::json val) {
    if (prop == K_W && val == 0) return sf::VideoMode::getDesktopMode().width;
    if (prop == K_H && val == 0) return sf::VideoMode::getDesktopMode().height;
    return val;
}

#define RETURN {\
    if (conf.find(prop) != conf.end()) return check(prop, conf[prop]);\
    return check(prop, default_conf[prop]);\
}

nlohmann::json Config::get(std::string prop) RETURN
std::string Config::getS(std::string prop) RETURN
int Config::getI(std::string prop) RETURN
bool Config::getB(std::string prop) RETURN

#undef RETURN
