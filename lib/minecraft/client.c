#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

char * downloadMainJar(cJSON * manifest, const char * versionName, const char * versionPath, CURL * session)
{
	
	char *url= NULL;
	char *path = malloc(1* sizeof(char *));
	char *sha1 = NULL;
	cJSON *temp = NULL;
	cJSON *jsonVersion = cJSON_GetObjectItemCaseSensitive(manifest, "downloads");
	strcpy(path, "");

	if (jsonVersion)
	{
		jsonVersion = cJSON_GetObjectItemCaseSensitive(jsonVersion, "client");
		
		temp = cJSON_GetObjectItemCaseSensitive(manifest, "id");

		temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "url");
		url = malloc((strlen(temp->valuestring)+1)* sizeof(char*));
		strcpy(url, temp->valuestring);
		
		temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "sha1");
		sha1 = malloc((strlen(temp->valuestring)+1)* sizeof(char*));
		strcpy(sha1, temp->valuestring);

		path = malloc((strlen(versionPath) + strlen(versionName) * 2 + strlen(".jar") + 2) * sizeof(char *));
		strcpy(path, versionPath);
		strcat(path, versionName);
		strcat(path, "/");
		strcat(path, versionName);
		strcat(path, ".jar");

		Http_Download(url, path, session);
	}
	return path;
}

char * getType(cJSON * manifest)
{
	char * type = "";
	cJSON *typeJson= cJSON_GetObjectItemCaseSensitive(manifest, "type");
	if (typeJson)
		type = typeJson->valuestring;
	return type;
}
