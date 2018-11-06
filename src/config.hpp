#ifndef _INSTIAPP_CONFIG_H
#define _INSTIAPP_CONFIG_H

#include <nlohmann/json.hpp>
#include <string>

/* Compile time constants */
#define BILLBOARD_VERSION "1.0.0"

#define CURL_TIMEOUT 10
#define LOGO_SCALE 0.3f

/* Config path */
#define CONFIG_PATH "config.json"

/* Key definitions */
#define K_LOCATION "location"                           // Location of billboard, sent in network requests

#define K_WINDOW_WIDTH "window_width"                   // Window width in pixels, set to 0 for native
#define K_WINDOW_HEIGHT "window_height"                 // Window height in pixels, set to 0 for native
#define K_WINDOW_TITLE "window_title"                   // Window height in pixels, set to 0 for native
#define K_FULLSCREEN "fullscreen"                       // Set true for fullscreen
#define K_TIME_DELAY "time_delay"                       // Time delay between transitions for EventScene
#define K_EVENTS_URL "url_events"                       // URL to fetch events
#define K_IMAGES_DIR "images_dir"                       // Directory to cache images in (should exist necessarily)
#define K_CACHE_DIR "cache_dir"                         // Directory to cache API responses (should exist necessarily)
#define K_REFRESH_DURATION "refresh_duration"           // Refresh events every x seconds
#define K_WEIGHT_THRESHOLD "weight_threshold"           // Don't display events below this weight

#define K_ANIMATION_ENABLED "animation_enabled"         // Set to false to disable animations
#define K_EVENT_ANIMATION_SPEED "animation_speed"       // Animation transition speed for EventScene

#define K_LOGO_SUPER "logo_super"                       // Text to display above logo (small)
#define K_LOGO_NAME "logo_name"                         // Title to display below logo (large)
#define K_LOGO_ANIM_SPEED "logo_anim_speed"             // Logo circle expand animation speed
#define K_LOGO_REPEAT_DELAY "logo_repeat_delay"         // Repeat showing logo after every x seconds
#define K_LOGO_SHOW_DURATION "logo_show_duration"       // Show logo for x seconds
#define K_LOGO_FADE_SPEED "logo_fade_speed"             // Speed of fading logo in/out

/* Short key definitions */
#define K_W "window_width"
#define K_H "window_height"

class Config {
    private:
    nlohmann::json conf;
    nlohmann::json default_conf;

    public:
    Config();
    nlohmann::json get(std::string prop);
    int getI(std::string prop);
    std::string getS(std::string prop);
    bool getB(std::string prop);
};

#endif
