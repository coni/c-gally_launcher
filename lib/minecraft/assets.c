#include "../cJSON/cJSON.h"
#include <string.h>
#include <stdlib.h>

char * getAssetIndex(cJSON *manifest)
{
	char *assetIndex = NULL;
	cJSON *index = cJSON_GetObjectItemCaseSensitive(manifest, "assetIndex");
	if (index)
	{
		index = cJSON_GetObjectItemCaseSensitive(index, "id");
		if (index)
		{
			assetIndex = malloc((strlen(index->valuestring)+1) * sizeof(char*));
			strcpy(assetIndex, index->valuestring);
		}
	}
	return assetIndex;
}
