#include "DataHandler.hpp"
#include <sys/stat.h>
#include <iostream>
#include <stdexcept>
using json = nlohmann::json;

void data_global_init() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void data_global_clean() {
    curl_global_cleanup();
}

inline bool file_exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

std::size_t callback(
    const char* in,
    std::size_t size,
    std::size_t num,
    std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

bool requestImage(std::string url) {
    CURL * curl;
    CURLcode imgresult;
    FILE *fp = NULL;

    curl = curl_easy_init();
    if (!curl) {
        throw std::invalid_argument("CURL not possible!");
    }

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    // Open file
    fp = fopen(imageFilename(url).c_str(), "wb");
    if (fp == NULL) std::cout << "File cannot be opened" << std::endl;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    // Grab image
    imgresult = curl_easy_perform(curl);
    if (imgresult) {
        std::cout << imgresult << "Cannot grab the image!\n";
    }

    // Clean up the resources
    curl_easy_cleanup(curl);

    // Close the file
    fclose(fp);
}

std::unique_ptr<std::string> request(std::string url) {
    // Init curl
    CURL * curl = curl_easy_init();
    if (!curl) {
        throw std::invalid_argument("CURL not possible!");
    }
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    // Response information.
    int httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        return httpData;
    }
    return NULL;
}

DataHandler::DataHandler() {}

DataHandler::~DataHandler() {}

std::vector<Event> DataHandler::getEvents() {
    auto eventsString = request(EVENTS_URL);
    json eventsJson = json::parse(*eventsString.get())["data"];

    std::vector<Event> eventVector;
    for (auto eventJson : eventsJson) {
        Event event(eventJson);

        if (event.imageUrl != STRING_EMPTY) {
            if (!file_exists(event.imageFileName)) {
                requestImage(event.imageUrl);
            }
        }

        eventVector.push_back(event);
    }
    return eventVector;
}
