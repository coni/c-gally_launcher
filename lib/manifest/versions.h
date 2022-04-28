#include "../cJSON/cJSON.h"
#include <curl/curl.h>

void MinecraftManifest_list_version(char * versionPath, cJSON *manifest, char * versionType);
cJSON * MinecraftManifest_get_all_versions_manifest(char * path, CURL * session);
cJSON * MinecraftManifest_get_version_manifest(cJSON **versionManifest, const char * version, const char * path, CURL * session);
