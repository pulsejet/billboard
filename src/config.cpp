#include "config.hpp"
#include <fstream>

Config::Config() {
    std::ifstream ifs(CONFIG_PATH);
    conf = nlohmann::json::parse(ifs);
}

#define RETURN {\
    return conf[prop];\
}

nlohmann::json Config::get(std::string prop) RETURN
std::string Config::getS(std::string prop) RETURN
int Config::getI(std::string prop) RETURN
bool Config::getB(std::string prop) RETURN

#undef RETURN
