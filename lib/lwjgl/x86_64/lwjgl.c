#include "../../cJSON/cJSON.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../utils/http.h"
#include "../../utils/json.h"
#include "../../utils/system.h"

extern char OSNAME[];
extern char ARCHITECTURE[];

cJSON * getBaseLwjglManifest(char * path, CURL * session)
{
  char filename[strlen(path) + 11];
  cJSON * manifest = NULL;
  strcpy(filename, path);
  strcat(filename, "lwjgl.json");
  Http_Download("https://gally-launcher.com/files/lwjgl.json", filename, session);
  parseJsonFile(filename, &manifest);
  return manifest;
}

int downloadLwjgl(char * lwjglVersion, char * path, CURL * session)
{
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
          char * fullpath = malloc((strlen(path) + strlen(filename->valuestring) + 1) * sizeof(char *));
          strcpy(fullpath, path);
          strcat(fullpath, filename->valuestring);

          Http_Download(url->valuestring, fullpath, session);
        }
      }
    }
  }
  return 0;
}
