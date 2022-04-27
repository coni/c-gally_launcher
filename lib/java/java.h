#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * MinecraftManifest_download_jre(cJSON * manifest, char * path, CURL * session);
