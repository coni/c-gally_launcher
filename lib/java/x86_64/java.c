#include "../../cJSON/cJSON.h"
#include "../../utils/system.h"
#include <stdlib.h>
#include "../../utils/http.h"
#include "../../utils/json.h"

char OSNAME[] = "linux";

cJSON * getAllJreManifest(char * path, CURL * session)
{
  char filename[strlen(path) + 8];
  cJSON * manifest = NULL;
  strcpy(filename, path);
  strcat(filename, "all.json");
  Http_Download("https://launchermeta.mojang.com/v1/products/java-runtime/2ec0cc96c44e5a76b9c8b7c39df7210883d12871/all.json", filename, session);

  parseJsonFile(filename, &manifest);
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
          Http_Download(url->valuestring, path, session);
          parseJsonFile(path, &jreManifest);
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

char * downloadJre(cJSON * manifest, char * path, CURL * session)
{
  char * tempPath = malloc((strlen(path) + 6) * sizeof(char *));
  strcpy(tempPath, path);
  strcat(tempPath, "temp/");

  char * component = getJreComponent(manifest);
  cJSON * jreAllManifest = getAllJreManifest(tempPath, session);
  cJSON * jreManifest = getJreManifest(jreAllManifest, component, tempPath, session);
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
            Http_Download(url->valuestring, fullpath, session);
            /* if (strcmp(executable, "True") == 0) */
            /*   // TO-DO */
          }
          else if (strcmp(type->valuestring, "directory") == 0)
            _mkdir(fullpath);
        }
      }
    }
  }

  return javaPath;
}
