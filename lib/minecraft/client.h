#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * downloadMainJar(cJSON * manifest, const char * versionPath, CURL * session);
