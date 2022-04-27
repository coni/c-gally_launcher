#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * MinecraftManifest_download_libraries(cJSON **manifest, const char *path, CURL **session);
char * MinecraftManifest_get_lwjgl_version(cJSON *manifest);
