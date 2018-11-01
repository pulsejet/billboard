#ifndef _INSTIAPP_COMMON_HPP
#define _INSTIAPP_COMMON_HPP

/* Common defines */
#define STRING_EMPTY ""

#include <iostream>
#include <string>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <sys/stat.h>
#include <iomanip>
#include "config.h"
#include "assets.h"

/* Commons */
extern const std::string monthNames[12];
extern const std::string dateSuffixes[10];

/** Get standard image file path */
inline std::string imageFilename(std::string url) {
    return IMAGES_DIR + url.substr(url.find_last_of("/") + 1);
}

/** Check if a file exists from path */
inline bool file_exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

/** Print current time to cout */
inline void print_time() {
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::cout << "[" << (now->tm_year + 1900) << '-'
         << (now->tm_mon + 1) << '-'
         << now->tm_mday
         << " "
         << std::setfill('0') << std::setw(2) << now->tm_hour << ":"
         << std::setfill('0') << std::setw(2) << now->tm_min << ":"
         << std::setfill('0') << std::setw(2) << now->tm_sec
         << "] ";
}

/** Load sprite for progress spinner */
inline sf::Sprite makeProgressSprite(sf::Texture * texture) {
    sf::Sprite sprite;
    if (texture->loadFromMemory(&progress_png, progress_png_len)) {
        texture->setSmooth(true);
        sprite.setTexture(*texture);
        const float scaling = (WINDOW_HEIGHT / (float) texture->getSize().y) * 0.1;
        sprite.setScale(scaling, scaling);
        sprite.setOrigin(texture->getSize().x / 2.0, texture->getSize().y / 2.0);
        sprite.setPosition(WINDOW_WIDTH * 0.85, WINDOW_HEIGHT * 0.85);
    }
    return sprite;
}

/** Convert date string to tm object */
tm getTime(std::string dateStr);

#endif
