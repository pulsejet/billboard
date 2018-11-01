#include "common.hpp"

const std::string monthNames[12] =  { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const std::string dateSuffixes[10] = { "th", "st", "nd", "rd", "th", "th", "th", "th", "th", "th" };

tm getTime(std::string dateStr) {
    int y,M,d,h,m;
    float s;
    int tzh = 0, tzm = 0;
    if (6 < sscanf(dateStr.c_str(), "%d-%d-%dT%d:%d:%f%d:%dZ", &y, &M, &d, &h, &m, &s, &tzh, &tzm)) {
        if (tzh < 0) {
        tzm = -tzm;    // Fix the sign on minutes.
        }
    }

    tm time;
    time.tm_year = y;
    time.tm_mon = M - 1;
    time.tm_mday = d;
    time.tm_hour = h;
    time.tm_min = m;
    time.tm_sec = (int) s;

    return time;
}
