#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * getAssetIndex(cJSON *manifest);
int downloadAssets(cJSON *manifest, char * path, CURL *session);

