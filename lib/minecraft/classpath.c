#include <curl/curl.h>
#include <regex.h>
#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include <stdlib.h>
#include <string.h>

int compareLwjglVersion(const char * new, const char * old)
{
	if (strlen(new) == strlen(old))
	{
		for (int i = 0; i < strlen(new); i++)
		{
			if (new[i] > old[i])
				return 0;
			else if (new[i] < old[i])
				return 1;
		}
		return 1;
	}
}
char *getClasspath_downloadLibraries(cJSON **manifest, const char *path, CURL **session)
{
	char *classpath = malloc(sizeof(char *));
	strcpy(classpath, "");

	const cJSON *lib = NULL;
	const cJSON *libraries = cJSON_GetObjectItemCaseSensitive(*manifest, "libraries");

	char *lwjglClasspath = malloc(sizeof(char *));
	strcpy(lwjglClasspath, "");

	char *lwjglVersion = malloc(sizeof(char *) * 5);
	strcpy(lwjglVersion, "0.0.0");

	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			char splittedLibName[strlen(libName->valuestring)];
			strcpy(splittedLibName, libName->valuestring);
			char *splittedLibNameElt = strtok(splittedLibName, ":");

			char *fullpath = malloc(strlen(path) + 1);
			char *org = NULL;
			char *name = NULL;
			char *version = NULL;
			int isLwjgl = 0;

			org = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(org, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");

			name = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(name, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");

			version = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(version, splittedLibNameElt);

			char *splitOrg = strtok(org, ".");	
			fullpath = realloc(fullpath, sizeof(char *) * (strlen(path) + strlen(org) + (strlen(name) + strlen(version)) * 2) + 8);
			strcpy(fullpath, path);
			
			while (splitOrg)
			{
				strcat(fullpath, splitOrg);
				strcat(fullpath, "/");
				if (strcmp(splitOrg, "lwjgl") == 0)
					isLwjgl = 1;
				splitOrg = strtok(NULL, ".");
			}

			strcat(fullpath, name);
			strcat(fullpath, "/");
			strcat(fullpath, version);
			strcat(fullpath, "/");
			strcat(fullpath, name);
			strcat(fullpath, "-");
			strcat(fullpath, version);
			strcat(fullpath, ".jar");
			
			if (isLwjgl)
			{
				if (strcmp(lwjglVersion, version) != 0)
				{
					if (compareLwjglVersion(version, lwjglVersion) == 0)
					{
						lwjglClasspath = realloc(lwjglClasspath, sizeof(char *) * (strlen(fullpath) + 1));
						strcpy(lwjglClasspath, fullpath);
						strcpy(lwjglVersion, version);
					}
					else
						continue;
				}
				lwjglClasspath = realloc(lwjglClasspath, sizeof(char *) * (strlen(lwjglClasspath) + strlen(fullpath) + 1));
				strcat(lwjglClasspath, ":");
				strcat(lwjglClasspath, fullpath);
			}
			else
			{
				classpath = realloc(classpath, sizeof(char *) * (strlen(classpath) + strlen(fullpath) + 1));
				strcat(classpath, fullpath);
				strcat(classpath, ":");
			}
				// Download Librarie
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
					
					Http_Download(libUrl->valuestring, fullpath, session);
				}
			}
		}
	}
	if (strcmp(lwjglClasspath, "") != 0)
	{
		classpath = realloc(classpath, strlen(classpath) + strlen(lwjglClasspath));
		strcat(classpath, lwjglClasspath);
	}
	return classpath;
}
