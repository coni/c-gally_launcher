#include <stddef.h>
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int Http_Download(const char * url, const char * filename, CURL **session);
