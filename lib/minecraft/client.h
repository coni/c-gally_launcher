#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * downloadMainJar(cJSON * manifest, const char * versionName, const char * versionPath, CURL * session);
