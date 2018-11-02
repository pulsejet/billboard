#ifndef _INSTIAPP_CONFIG_H
#define _INSTIAPP_CONFIG_H

#include <nlohmann/json.hpp>
#include <string>

#define CONFIG_PATH "config.json"

#define K_WINDOW_WIDTH "window_width"
#define K_WINDOW_HEIGHT "window_height"
#define K_WINDOW_TITLE "window_title"
#define K_FULLSCREEN "fullscreen"
#define K_TIME_DELAY "time_delay"
#define K_EVENTS_URL "url_events"
#define K_IMAGES_DIR "images_dir"
#define K_REFRESH_DURATION "refresh_duration"
#define K_WEIGHT_THRESHOLD "weight_threshold"

#define K_ANIMATION_ENABLED "animation_enabled"
#define K_EVENT_ANIMATION_SPEED "animation_speed"

class Config {
    private:
    nlohmann::json conf;

    public:
    Config();
    nlohmann::json get(std::string prop);
    int getI(std::string prop);
    std::string getS(std::string prop);
    bool getB(std::string prop);
};

#endif
