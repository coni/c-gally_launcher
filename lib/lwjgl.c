#include "cJSON/cJSON.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/http.h"
#include "utils/json.h"
#include "utils/system.h"

extern const char OSNAME[256];
extern const char ARCHITECTURE[256];

cJSON * getBaseLwjglManifest(char * path, CURL * session)
{
  char filename[strlen(path) + 11];
  cJSON * manifest = NULL;
  strcpy(filename, path);
  strcat(filename, "lwjgl.json");
  http_download("https://gally-launcher.com/files/lwjgl.json", filename, session);
  return json_parse_file(filename);
}

char * download_lwjgl(char * lwjglVersion, char * path, CURL * session)
{
	char * lwjglPath = malloc((strlen(path) + strlen(lwjglVersion) + 2)*sizeof(char*));
	strcpy(lwjglPath, path);
	strcat(lwjglPath, lwjglVersion);
	strcat(lwjglPath, "/");

  cJSON * lwjglManifest = getBaseLwjglManifest(path, session);
  cJSON * i = NULL;
  lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, "lwjgl");
  if (lwjglManifest)
  {
    lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, OSNAME);
    if (lwjglVersion)
    {
      lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, ARCHITECTURE);
      if (lwjglManifest)
      {
        lwjglManifest = cJSON_GetObjectItemCaseSensitive(lwjglManifest, lwjglVersion);
        cJSON_ArrayForEach(i, lwjglManifest)
        {
          cJSON * url = cJSON_GetObjectItemCaseSensitive(i, "url");
          cJSON * filename = cJSON_GetObjectItemCaseSensitive(i, "filename");
          char * fullpath = malloc((strlen(lwjglPath) + strlen(filename->valuestring) + 1) * sizeof(char *));
          strcpy(fullpath, lwjglPath);
          strcat(fullpath, filename->valuestring);
          http_download(url->valuestring, fullpath, session);
        }
      }
    }
  }
  return lwjglPath;
}
