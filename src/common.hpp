#ifndef _INSTIAPP_COMMON_HPP
#define _INSTIAPP_COMMON_HPP

/* Common defines */
#define STRING_EMPTY ""

#include <iostream>
#include <string>
#include <ctime>
#include <sys/stat.h>
#include <iomanip>
#include "config.h"

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

/** Convert date string to tm object */
tm getTime(std::string dateStr);

#endif
