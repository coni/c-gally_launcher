#include "../cJSON/cJSON.h"
#include <curl/curl.h>

char * downloadJre(cJSON * manifest, char * path, CURL * session);
char * getJreComponent(cJSON * manifest);
cJSON * getJreManifest(cJSON * manifest, char * component, char * filename, CURL * session);
cJSON * getAllJreManifest(char * path, CURL * session);
