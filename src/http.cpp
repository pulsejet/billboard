#include <curl/curl.h>
#include <fstream>
#include "http.hpp"
#include "common.hpp"

/** Callback for getting bytes from request */
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

/** Request and save an image to common location */
bool requestImage(Config * cfg, std::string url) {
    CURL * curl;
    CURLcode imgresult;
    FILE *fp = NULL;
    bool success = false;

    curl = curl_easy_init();
    if (!curl) {
        throw std::invalid_argument("CURL not possible!");
    }

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURL_TIMEOUT);

    // Open file
    fp = fopen(imageFilename(cfg, url).c_str(), "wb");
    if (fp == NULL) std::cout << "File cannot be opened" << std::endl;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    // Grab image
    imgresult = curl_easy_perform(curl);
    if (imgresult) {
        std::cout << imgresult << "Cannot grab the image!\n";
    } else {
        success = file_exists(imageFilename(cfg, url).c_str());
    }

    // Clean up the resources
    curl_easy_cleanup(curl);

    int httpCode(0);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    print_time();
    std::cout << httpCode << " " << url << std::endl;

    // Close the file
    fclose(fp);

    return success;
}

/** Get response of an HTTP request as string ptr */
std::unique_ptr<std::string> requestStr(Config * cfg, std::string url) {
    // Init curl
    CURL * curl = curl_easy_init();
    if (!curl) {
        throw std::invalid_argument("CURL not possible!");
    }

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURL_TIMEOUT);

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

    print_time();
    std::cout << httpCode << " " << url << std::endl;

    // Get cache file name
    std::string cachefile = cfg->getS(K_CACHE_DIR) +  std::to_string(slash_hash(url.c_str()));

    // Check status code
    if (httpCode == 200) {
        // Cache response
        std::ofstream outs(cachefile);
        outs << *httpData.get();
        outs.close();

        // Return for further processing
        return httpData;
    } else if (file_exists(cachefile)) {
        // Fallback to cache if it exists
        std::ifstream ifs(cachefile);

        // Read cache
        std::unique_ptr<std::string> cache;
        cache.reset(new std::string((std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>())));
        ifs.close();

        return cache;
    }

    return NULL;
}
