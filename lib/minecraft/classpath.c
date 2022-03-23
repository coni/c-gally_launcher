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
	regex_t regex;
	regcomp(&regex, "(.*)?org/lwjgl/(.*)?/([0-9.].*)/(.*)?", REG_EXTENDED);
	int reti;

	int s_classpath = 1;
	char *classpath = malloc(sizeof(char *) * s_classpath);
	strcpy(classpath, "");
	const cJSON *lib = NULL;
	const cJSON *libraries = cJSON_GetObjectItemCaseSensitive(*manifest, "libraries");
	int s_lwjglClasspath = 1;
	char *lwjglClasspath = malloc(s_lwjglClasspath*sizeof(char *));
	char *lwjglVersion = malloc(5 * sizeof(char *));
	strcpy(lwjglVersion, "0.0.0");

	if (libraries)
	{
		cJSON_ArrayForEach(lib, libraries)
		{
			cJSON *libName = cJSON_GetObjectItemCaseSensitive(lib, "name");
			// Formatting Classpath pour Fabric, garde ce code
			char splittedLibName[strlen(libName->valuestring)];
			strcpy(splittedLibName, libName->valuestring);
			char *splittedLibNameElt = strtok(splittedLibName, ":");
			char *fullpath = malloc(strlen(path) + 1);
			char *org = NULL;
			char *lib = NULL;
			char *version = NULL;

			int counter = 0;
			while (splittedLibNameElt != NULL)
			{
				if (counter == 0)
				{
					org = malloc(sizeof(char *) * strlen(splittedLibNameElt));
					strcpy(org, splittedLibNameElt);
				}
				else if (counter == 1)
				{
					lib = malloc(sizeof(char *) * strlen(splittedLibNameElt));
					strcpy(lib, splittedLibNameElt);
				}
				else if (counter == 2)
				{
					version = malloc(sizeof(char *) * strlen(splittedLibNameElt));
					strcpy(version, splittedLibNameElt);
				}

				splittedLibNameElt =  strtok(NULL,":");
				counter++;
			}

			char *splitOrg = strtok(org, ".");
			s_classpath = strlen(path) + s_classpath + (strlen(lib) + strlen(version)) * 2 + 8;
			classpath = realloc(classpath, sizeof(char *) * s_classpath);
			while (splitOrg)
			{
				s_classpath = s_classpath + strlen(splitOrg) + 1;
				classpath = realloc(classpath, sizeof(char *)*s_classpath);
				strcat(classpath, splitOrg);
				strcat(classpath, "/");
				splitOrg = strtok(NULL, ".");
			}
			strcpy(fullpath, path);
			strcat(classpath, lib);
			strcat(classpath, "/");
			strcat(classpath, version);
			strcat(classpath, "/");
			strcat(classpath, lib);
			strcat(classpath, "-");
			strcat(classpath,version);
			strcat(classpath, ".jar");
			strcat(classpath, ":");
			printf("%s\n", classpath);

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
	/* classpath = realloc(classpath, s_classpath+s_lwjglClasspath); */
	/* strcat(classpath, lwjglClasspath); */
	/* free(lwjglVersion); */
	return classpath;
}
