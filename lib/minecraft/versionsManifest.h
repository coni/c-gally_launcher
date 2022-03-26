#include "../cJSON/cJSON.h"
#include <curl/curl.h>

int downloadMinecraftVersion(cJSON **versionManifest, const char * version, const char * path, CURL * session);
