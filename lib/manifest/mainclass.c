#include "../cJSON/cJSON.h"
#include <string.h>
#include <stdlib.h>

char *MinecraftManifest_get_mainclass(cJSON *manifest)
{
	char *mainclass = NULL;
	cJSON *jsonMainclass = cJSON_GetObjectItemCaseSensitive(manifest, "mainClass");
	mainclass = malloc(strlen(jsonMainclass->valuestring));
	strcpy(mainclass, jsonMainclass->valuestring);
	return mainclass;
}
