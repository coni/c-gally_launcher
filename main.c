#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "lib/cJSON/cJSON.h"
#include "lib/utils/http.h"


void parseJsonFile(const char *filename, cJSON **json)
{ 
	int c;
	int index = 0;
	int size = 2;
  char *content = (char *) malloc(size);
	FILE *fp;

  fp = fopen(filename, "r");
	while ((c = getc(fp)) != EOF)
	{
		if (index >= size-1)
			content = (char *) realloc(content, size++);
		content[index] = c;
		index++;
	}
	fclose(fp);

  *json = cJSON_Parse(content);
	free(content);
} 

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


int main()
{
	CURL *session = curl_easy_init();
  cJSON *versionManifest = NULL;
	char *url, *sha1, *type;
	Http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", "version_manifest_v2.json", session);
  parseJsonFile("version_manifest_v2.json", &versionManifest);
  getMinecraftVersion(&versionManifest,"1.18.2", &url, &sha1, &type);
	printf("%s\n%s\n%s", type, sha1, url);
	Http_Download(url,"1.18.2.json", session);
	return 0;
}
