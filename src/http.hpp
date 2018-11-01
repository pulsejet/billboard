#ifndef _INSTIAPP_HTTP_HPP
#define _INSTIAPP_HTTP_HPP

#include <iostream>
#include <memory>

/** Request and save an image to common location */
bool requestImage(std::string url);

/** Get response of an HTTP request as string unique_ptr */
std::unique_ptr<std::string> requestStr(std::string url);

#endif
