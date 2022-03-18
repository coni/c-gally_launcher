#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char *getClasspath_downloadLibraries(cJSON **manifest, const char *path, CURL **session);
