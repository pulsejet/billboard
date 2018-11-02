#include "config.hpp"
#include <fstream>
#include <SFML/Window.hpp>

Config::Config() {
    std::ifstream ifs(CONFIG_PATH);
    conf = nlohmann::json::parse(ifs);
    ifs.close();

    /* Set fallbacks for width and height */
    if (this->getI(K_WINDOW_WIDTH) == 0) {
        conf[K_WINDOW_WIDTH] = sf::VideoMode::getDesktopMode().width;
    }

    if (this->getI(K_WINDOW_HEIGHT) == 0) {
        conf[K_WINDOW_HEIGHT] = sf::VideoMode::getDesktopMode().height;
    }
}

#define RETURN {\
    return conf[prop];\
}

nlohmann::json Config::get(std::string prop) RETURN
std::string Config::getS(std::string prop) RETURN
int Config::getI(std::string prop) RETURN
bool Config::getB(std::string prop) RETURN

#undef RETURN
