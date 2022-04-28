#include "../cJSON/cJSON.h"
#include "../utils/system.h"
#include "../utils/http.h"
#include "../utils/json.h"
#include <string.h>
#include <stdlib.h>

cJSON * MinecraftManifest_get_all_versions_manifest(char * path, CURL * session)
{
	char * fullpath = malloc((strlen(path) + 25) * sizeof(char *));
	strcpy(fullpath, path);
	strcat(fullpath, "version_manifest_v2.json");
	http_download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", fullpath, session);
  return json_parse_file(fullpath);
}

void MinecraftManifest_list_version(char * versionPath, cJSON *manifest, char * versionType)
{
	cJSON * i = NULL;
	manifest = cJSON_GetObjectItemCaseSensitive(manifest, "versions");
	if (strcmp(versionType, "downloaded") == 0)
	{
		int arraySize = system_count_files(versionPath, 2);
		char *listDownloadedVersion[arraySize];
		system_ls(versionPath, 2, listDownloadedVersion);	
		for (int i = 0; i < arraySize; i++)
		{
			char jsonFile[strlen(versionPath) + strlen(listDownloadedVersion[i]) * 2 + 7];
			strcpy(jsonFile, versionPath);
			strcat(jsonFile, listDownloadedVersion[i]);
			strcat(jsonFile, "/");
			strcat(jsonFile, listDownloadedVersion[i]);
			strcat(jsonFile, ".json");
			if (system_file_exist(jsonFile) == 0)
				printf("%s\n", listDownloadedVersion[i]);
		}
	}
	else if (manifest)
	{
		for (int j = cJSON_GetArraySize(manifest)-1; j >= 0; j--)
		{
			i = cJSON_GetArrayItem(manifest, j);
			cJSON * type = cJSON_GetObjectItemCaseSensitive(i, "type");	
			cJSON * id = cJSON_GetObjectItemCaseSensitive(i, "id");	
			if (strcmp(type->valuestring, versionType) == 0)
				printf("%s\n", id->valuestring);
		}
	}
}

cJSON * MinecraftManifest_get_version_manifest(cJSON **versionManifest, const char * version, char * path, CURL * session)
{
	int status = 1;
	char * fullpath = malloc((strlen(path) + strlen(version)*2 + 7)*sizeof(char*));
	strcpy(fullpath, path);
	strcat(fullpath, version);
	strcat(fullpath, "/");
	strcat(fullpath, version);
	strcat(fullpath, ".json");

	const cJSON *versions = NULL;
	const cJSON *versionInfo  = NULL;

	versions = cJSON_GetObjectItemCaseSensitive(*versionManifest, "versions");
	cJSON_ArrayForEach(versionInfo, versions)
	{
		cJSON *id = cJSON_GetObjectItemCaseSensitive(versionInfo, "id");
		cJSON *url = cJSON_GetObjectItemCaseSensitive(versionInfo, "url");
		if (strcmp(id->valuestring,version) == 0)
		{
			http_download(url->valuestring, fullpath, session);
			status = 0;
			break;
		}
	}
	return json_parse_file(fullpath);
}

