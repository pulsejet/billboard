#ifndef _INSTIAPP_HTTP_HPP
#define _INSTIAPP_HTTP_HPP

#include <iostream>
#include <memory>
#include "config.hpp"

/** Request and save an image to common location */
bool requestImage(Config * cfg, std::string url);

/** Get response of an HTTP request as string unique_ptr */
std::unique_ptr<std::string> requestStr(Config * cfg, std::string url);

#endif
