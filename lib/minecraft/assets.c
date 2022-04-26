#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include "../utils/json.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

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

cJSON *downloadAssetsManifest(cJSON *manifest, char *path, CURL *session)
{
	cJSON *i = cJSON_GetObjectItemCaseSensitive(manifest, "assetIndex");
	cJSON *assetsManifest = NULL;
	char * fullpath = NULL;
	if (i)
	{
		cJSON *url = cJSON_GetObjectItemCaseSensitive(i, "url");
		cJSON *index = cJSON_GetObjectItemCaseSensitive(i, "id");
		fullpath = malloc((strlen(path) + strlen(index->valuestring) + 1) * sizeof(char *));
		strcpy(fullpath, path);
		strcat(fullpath, index->valuestring);
		Http_Download(url->valuestring, fullpath, session);
		parseJsonFile(fullpath, &assetsManifest);
	}

	return assetsManifest;
}

int downloadAssets(cJSON *manifest, char * assetsPath, CURL *session)
{
	char ressourceUrl[] = "https://resources.download.minecraft.net/";
	char * path = malloc((strlen(assetsPath) + 8) * sizeof(char *));
	strcpy(path, assetsPath);
	path = strcat(path, "objects/");
	cJSON *assetsManifest = downloadAssetsManifest(manifest, path, session);
	cJSON *i = NULL;
	assetsManifest = cJSON_GetObjectItemCaseSensitive(assetsManifest, "objects");
	if (assetsManifest)
	{
		cJSON_ArrayForEach(i, assetsManifest)
		{
			cJSON *hash = cJSON_GetObjectItemCaseSensitive(i, "hash");
			char folder[3];
			strcpy(folder, "");
			folder[0] = hash->valuestring[0];
			folder[1] = hash->valuestring[1];
			char *url = malloc((strlen(hash->valuestring) + 4)*sizeof(char*));
			strcpy(url, ressourceUrl);
			strcat(url, folder);
			strcat(url, "/");
			strcat(url, hash->valuestring);

			char * fullpath = malloc(((strlen(hash->valuestring) + strlen(folder)) + 1) *  sizeof(char *));
			strcpy(fullpath, path);
			strcat(fullpath, folder);
			strcat(fullpath, "/");
			strcat(fullpath, hash->valuestring);
			Http_Download(url, fullpath, session);
		}
	}
	return 0;
}
