#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

char * downloadMainJar(cJSON * manifest, const char * versionPath, CURL * session)
{
	
	char *url= NULL;
	char *path = malloc(1* sizeof(char *));
	char *sha1 = NULL;
	char *version = NULL;
	cJSON *temp = NULL;
	cJSON *jsonVersion = cJSON_GetObjectItemCaseSensitive(manifest, "downloads");
	strcpy(path, "");

	jsonVersion = cJSON_GetObjectItemCaseSensitive(jsonVersion, "client");
	
	temp = cJSON_GetObjectItemCaseSensitive(manifest, "id");
	version = malloc((strlen(temp->valuestring)+1)* sizeof(char*));
	strcpy(version, temp->valuestring);

	temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "url");
	url = malloc((strlen(temp->valuestring)+1)* sizeof(char*));
	strcpy(url, temp->valuestring);
	
	temp = cJSON_GetObjectItemCaseSensitive(jsonVersion, "sha1");
	sha1 = malloc((strlen(temp->valuestring)+1)* sizeof(char*));
	strcpy(sha1, temp->valuestring);

	path = malloc((strlen(versionPath) + strlen(version) * 2 + strlen(".jar") + 2) * sizeof(char *));
	strcpy(path, versionPath);
	strcat(path, version);
	strcat(path, "/");
	strcat(path, version);
	strcat(path, ".jar");

	Http_Download(url, path, session);
	return path;
}
