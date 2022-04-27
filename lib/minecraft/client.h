#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * MinecraftManifest_download_client(cJSON * manifest, const char * versionName, const char * versionPath, CURL * session);
