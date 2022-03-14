#include <stdio.h>
#include <string.h>
#include "lib/cJSON/cJSON.h"
#include <stdlib.h>


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
  cJSON *versionManifest = NULL;
  parseJsonFile("version_manifest_v2.json", &versionManifest);

	char *url, *sha1, *type;
  getMinecraftVersion(&versionManifest,"1.18.2", &url, &sha1, &type);

	return 0;
}
