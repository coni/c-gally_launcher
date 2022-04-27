#include "../../cJSON/cJSON.h"
#include "../../utils/system.h"
#include <stdlib.h>
#include "../../utils/http.h"
#include "../../utils/json.h"

extern char OSNAME[];
extern char ARCHITECTURE[];

cJSON * getBaseJreManifest(char * path, CURL * session)
{
  char filename[strlen(path) + 8];
  cJSON * manifest = NULL;
  strcpy(filename, path);
  strcat(filename, "all.json");
  http_download("https://launchermeta.mojang.com/v1/products/java-runtime/2ec0cc96c44e5a76b9c8b7c39df7210883d12871/all.json", filename, session);

  manifest = json_parse_file(filename);
  return manifest;
}

cJSON * getJreManifest(cJSON * manifest, char * component, char * path, CURL * session)
{
  cJSON * jreManifest = NULL;
  cJSON * i = NULL;
  path = realloc(path, (strlen(path) + strlen(component) + 6));
  manifest = cJSON_GetObjectItemCaseSensitive(manifest, OSNAME);
  strcat(path, component);
  strcat(path, ".json");
  if (manifest)
  {
    manifest = cJSON_GetObjectItemCaseSensitive(manifest, component);
    if (manifest)
    {
      cJSON_ArrayForEach(i, manifest)
      {
        manifest = cJSON_GetObjectItemCaseSensitive(i, "manifest");
        if (manifest)
        {
          cJSON * url = cJSON_GetObjectItemCaseSensitive(manifest, "url");
          http_download(url->valuestring, path, session);
          jreManifest = json_parse_file(path);
        }
      }
    }
  }
  return jreManifest;
}

char * getJreComponent(cJSON * manifest)
{
  char * component = NULL;
  cJSON * i = cJSON_GetObjectItemCaseSensitive(manifest, "javaVersion");
  if (i)
  {
    
    i = cJSON_GetObjectItemCaseSensitive(i, "component");
    if (i)
    {
      component = i->valuestring;
    }
  }
  return component;
}

char * MinecraftManifest_download_jre(cJSON * manifest, char * path, CURL * session)
{
  char * tempPath = malloc((strlen(path) + 6) * sizeof(char *));
  strcpy(tempPath, path);
  strcat(tempPath, "temp/");

  char * component = getJreComponent(manifest);
  cJSON * jreBaseManifest = getBaseJreManifest(tempPath, session);
  cJSON * jreManifest = getJreManifest(jreBaseManifest, component, tempPath, session);
  char * javaPath = malloc((strlen(path) + strlen(component)*2 + strlen(OSNAME) + 4) * sizeof(char *));
  strcpy(javaPath, path);
  strcat(javaPath, component);
  strcat(javaPath, "/");
  strcat(javaPath, OSNAME);
  strcat(javaPath, "/");
  strcat(javaPath, component);
  strcat(javaPath, "/");

  cJSON * element = NULL;

  if (jreManifest)
  {
    jreManifest = cJSON_GetObjectItemCaseSensitive(jreManifest, "files");
    if (jreManifest)
    {
      cJSON_ArrayForEach(element, jreManifest)
      {
        char * filename = element->string;
        cJSON * type = cJSON_GetObjectItemCaseSensitive(element, "type");
        cJSON * executable = cJSON_GetObjectItemCaseSensitive(element, "executable");
        cJSON * downloads = cJSON_GetObjectItemCaseSensitive(element, "downloads");

        downloads = cJSON_GetObjectItemCaseSensitive(downloads, "raw");
        cJSON * url = cJSON_GetObjectItemCaseSensitive(downloads,"url");
        char fullpath[(strlen(javaPath) + strlen(filename) + 1)];
        strcpy(fullpath, javaPath);
        strcat(fullpath, filename);
        if (type)
        {
          if (strcmp(type->valuestring, "file") == 0)
          {
            http_download(url->valuestring, fullpath, session);
            if (executable)
            {
                if (executable->valueint == 1)
                  system_makeExec(fullpath);
            }
          }
          else if (strcmp(type->valuestring, "directory") == 0)
            _mkdir(fullpath);
        }
      }
    }
  }

  return javaPath;
}
