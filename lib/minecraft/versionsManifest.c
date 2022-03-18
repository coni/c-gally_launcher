#include "../cJSON/cJSON.h"
#include <string.h>
int getMinecraftVersion(cJSON **versionManifest, const char * version, char ** Url, char ** Sha1, char ** Type)
{
	int status = 1;
	const cJSON *versions = NULL;
	const cJSON *versionInfo  = NULL;
	versions = cJSON_GetObjectItemCaseSensitive(*versionManifest, "versions");
	cJSON_ArrayForEach(versionInfo, versions)
	{
		cJSON *id = cJSON_GetObjectItemCaseSensitive(versionInfo, "id");
		cJSON *url = cJSON_GetObjectItemCaseSensitive(versionInfo, "url");
		cJSON *type = cJSON_GetObjectItemCaseSensitive(versionInfo, "type");
		cJSON *sha1 = cJSON_GetObjectItemCaseSensitive(versionInfo, "sha1");
		if (strcmp(id->valuestring,version) == 0)
		{
			*Url = url->valuestring;
			*Sha1 = sha1->valuestring;
			*Type = type->valuestring;
			status = 0;
			break;
		}
	}
	return status;
}

