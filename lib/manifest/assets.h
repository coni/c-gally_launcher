#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * MinecraftManifest_get_asset_index(cJSON *manifest);
int MinecraftManifest_download_assets(cJSON *manifest, char * path, CURL *session);

