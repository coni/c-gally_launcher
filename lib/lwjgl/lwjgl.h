#include "../cJSON/cJSON.h"
#include <curl/curl.h>

int downloadLwjgl(char * lwjglVersion, char * path, CURL * session);
cJSON * getBaseLwjglManifest(char * path, CURL * session);
