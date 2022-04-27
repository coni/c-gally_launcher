#include <stddef.h>
#include <curl/curl.h>

int http_download(const char * url, const char * filename, CURL **session);
