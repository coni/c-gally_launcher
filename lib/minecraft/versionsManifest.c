#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include <string.h>

int downloadMinecraftVersion(cJSON **versionManifest, const char * version, char * path, CURL * session)
{
	int status = 1;
	const cJSON *versions = NULL;
	const cJSON *versionInfo  = NULL;
	versions = cJSON_GetObjectItemCaseSensitive(*versionManifest, "versions");
	cJSON_ArrayForEach(versionInfo, versions)
	{
		cJSON *id = cJSON_GetObjectItemCaseSensitive(versionInfo, "id");
		cJSON *url = cJSON_GetObjectItemCaseSensitive(versionInfo, "url");
		if (strcmp(id->valuestring,version) == 0)
		{
			Http_Download(url->valuestring, path, session);
			status = 0;
			break;
		}
	}
	return status;
}

