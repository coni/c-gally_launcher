#include <regex.h>
#include "../cJSON/cJSON.h"
#include "../utils/http.h"
#include <stdlib.h>
#include <string.h>

int compareLwjglVersion(const char * new, const char * old)
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

char * getLwjglVersion(cJSON *manifest)
{
	const cJSON *libraries = cJSON_GetObjectItemCaseSensitive(manifest, "libraries");
	cJSON * lib = NULL;
	char *lwjglVersion = malloc(sizeof(char *) * 5);
	strcpy(lwjglVersion, "0.0.0");
	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			int isLwjgl = 0;
			char *version = NULL;
			char splittedLibName[strlen(libName->valuestring)];
			strcpy(splittedLibName, libName->valuestring);
			char *splittedLibNameElt = strtok(splittedLibName, ":");
			char *org = NULL;
			

			org = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(org, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");

			char *name = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(name, splittedLibNameElt);
			splittedLibNameElt =  strtok(NULL,":");
			
			version = malloc(sizeof(char *) * strlen(splittedLibNameElt));
			strcpy(version, splittedLibNameElt);
			
			char *splitOrg = strtok(org, ".");
			char *libNameFormatted = malloc(sizeof (char *) * ((strlen(name) + strlen(version))* 2 + 8));
			strcpy(libNameFormatted, "");

			
			while (splitOrg)
			{
				strcat(libNameFormatted, splitOrg);
				strcat(libNameFormatted, "/");
				if (strcmp(splitOrg, "lwjgl") == 0)
					isLwjgl = 1;
				splitOrg = strtok(NULL, ".");
			}

			if (isLwjgl)
			{
				version[5] = '\0';
				if (strcmp(lwjglVersion, version) != 0)
				{
					if (compareLwjglVersion(version, lwjglVersion) == 0)
					{
						strcpy(lwjglVersion, version);
					}
					else
						continue;
				}
			}
		}
	}
	return lwjglVersion;
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
			char *libNameFormatted = NULL;

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
			libNameFormatted = malloc(sizeof (char *) * ((strlen(name) + strlen(version))* 2 + 8));
			strcpy(libNameFormatted, "");

			while (splitOrg)
			{
				strcat(libNameFormatted, splitOrg);
				strcat(libNameFormatted, "/");
				if (strcmp(splitOrg, "lwjgl") == 0)
					isLwjgl = 1;
				splitOrg = strtok(NULL, ".");
			}

			strcat(libNameFormatted, name);
			strcat(libNameFormatted, "/");
			strcat(libNameFormatted, version);
			strcat(libNameFormatted, "/");
			strcat(libNameFormatted, name);
			strcat(libNameFormatted, "-");
			strcat(libNameFormatted, version);
			strcat(libNameFormatted, ".jar");
			
			fullpath = realloc(fullpath, sizeof(char *) * (strlen(path) + strlen(libNameFormatted)));
			strcpy(fullpath, path);
			strcat(fullpath, libNameFormatted);
			
			if (isLwjgl)
			{
				version[5] = '\0';
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
			} else
			{
				libDlInfo = cJSON_GetObjectItemCaseSensitive(lib, "url");
				if (libDlInfo)
				{
					char *libUrl = malloc(sizeof(char *) * (strlen(libDlInfo->valuestring) + strlen(libNameFormatted) + 1));
					strcpy(libUrl, libDlInfo->valuestring);
					strcat(libUrl, libNameFormatted);
					Http_Download(libDlInfo->valuestring, fullpath, session);	
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
