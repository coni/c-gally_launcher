#include "../cJSON/cJSON.h"
#include <curl/curl.h>
#include "../utils/http.h"
#include <stdlib.h>
#include <string.h>

char *getClasspath_downloadLibraries(cJSON **manifest, const char *path, CURL **session)
{
	int s_classpath = 1;
	char *classpath = malloc(sizeof(char *) * s_classpath);
	strcpy(classpath, "");
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
					
					char splittedLibName[strlen(libName->valuestring)];
					strcpy(splittedLibName, libName->valuestring);
					char *splittedLibNameElt = strtok(splittedLibName, ":");
					
					// Formatting Classpath
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
					while (splitOrg)
					{
						s_classpath = s_classpath + strlen(splitOrg) + 1;
						classpath = realloc(classpath, sizeof(char *)*s_classpath);
						strcat(classpath, splitOrg);
						strcat(classpath, "/");
						splitOrg = strtok(NULL, ".");
					}
					s_classpath = s_classpath + (strlen(lib) + strlen(version)) * 2 + 8;
					classpath = realloc(classpath, sizeof(char *) * s_classpath);
					strcat(classpath, lib);
					strcat(classpath, "/");
					strcat(classpath, version);
					strcat(classpath, "/");
					strcat(classpath, lib);
					strcat(classpath, "-");
					strcat(classpath,version);
					strcat(classpath, ".jar");
					strcat(classpath, ":");

					// Download Librarie
					Http_Download(libUrl->valuestring, fullpath, session);
				}
			}
		}
	}
	classpath[s_classpath-2] = '\0';
	return classpath;
}
