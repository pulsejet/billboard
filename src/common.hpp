#ifndef _INSTIAPP_COMMON_HPP
#define _INSTIAPP_COMMON_HPP

/* Common defines */
#define STRING_EMPTY ""

#include <string>
#include <ctime>
#include <sys/stat.h>
#include "config.h"

/* Commons */
extern const std::string monthNames[12];
extern const std::string dateSuffixes[10];

/* Assets */
extern unsigned char fade_png[];
extern unsigned int fade_png_len;

extern unsigned char roboto_light_ttf[];
extern unsigned int roboto_light_ttf_len;

extern unsigned char roboto_ttf[];
extern unsigned char roboto_ttf_len;

/** Get standard image file path */
inline std::string imageFilename(std::string url) {
    return IMAGES_DIR + url.substr(url.find_last_of("/") + 1);
}

/** Check if a file exists from path */
inline bool file_exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

/** Convert date string to tm object */
tm getTime(std::string dateStr);

#endif
