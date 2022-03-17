#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "lib/cJSON/cJSON.h"
#include "lib/utils/http.h"
#include "lib/utils/system.h"

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

char *getClasspath_downloadLibraries(cJSON **manifest, const char *path, CURL **session)
{
	char *classpath = NULL;
	const cJSON *lib = NULL;
	const cJSON *libraries = cJSON_GetObjectItemCaseSensitive(*manifest, "libraries");
	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			cJSON *libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "downloads");
			if (libDlInfo)
			{	
				libDlInfo = cJSON_GetObjectItemCaseSensitive(libDlInfo, "artifact");
				if (libDlInfo)
				{
					cJSON *libUrl = cJSON_GetObjectItemCaseSensitive(libDlInfo, "url");
					cJSON *libSha1 = cJSON_GetObjectItemCaseSensitive(libDlInfo, "sha1");
					cJSON *libPath = cJSON_GetObjectItemCaseSensitive(libDlInfo, "path");
					cJSON *libSize = cJSON_GetObjectItemCaseSensitive(libDlInfo, "size");
					char *fullpath = malloc(strlen(path)+strlen(libPath->valuestring)+1);	
					strcpy(fullpath, "");
					strcat(fullpath, path);
					strcat(fullpath, libPath->valuestring);
					
					// TODO : formatting libName for the Classpath
					// 				And put theses functions in another file

					Http_Download(libUrl->valuestring, fullpath, session);
				}
			}
		}
	}
	return "hihi";
}

int main()
{
	// TODO Functions:
	// getClasspath_downloadLibraries -> Download Minecraft Libraries. 																Param: manifest, path, session. Return: (char *)
	// getGameArguments 							-> Get required Minecraft arguments in order to start the game. Param: manifest. Return: (char *)
	// getJavaArguments								-> Get Java argument required by Minecraft. 										Param: manifest. Return Arguments (char *)
	// getMainclass										-> Get Minecraft's mainclass. 																	Param: manifest. Return mainclass (char *)
	// downloadAssets									-> Download Minecraft Assets. 																	Param: manifest, session.
	// downloadJava										-> Download Java Runtime.																				Param: manifest, session.

	// TODO Other Functions:
	// http.c													-> GET, POST request.
	// system.c												-> get OS
	// Globally												-> Adapt function for Windows

	CURL *session = curl_easy_init();
  cJSON *versionsManifest = NULL;
	cJSON *versionManifest = NULL;
	char *url, *sha1, *type = NULL;

	Http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", "trash/version_manifest_v2.json", session);
  parseJsonFile("trash/version_manifest_v2.json", &versionsManifest);
  getMinecraftVersion(&versionsManifest,"1.12.2", &url, &sha1, &type);

	Http_Download(url, "trash/1.18.2.json", session);
  parseJsonFile("trash/1.18.2.json", &versionManifest);

	getClasspath_downloadLibraries(&versionManifest, "trash/minecraft/libraries/", session);
	return 0;
}
