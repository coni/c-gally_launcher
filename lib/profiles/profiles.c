#include "../cJSON/cJSON.h"
#include "../utils/json.h"
#include "../utils/system.h"
#include <string.h>
#include <stdlib.h>

cJSON * profiles_initialization()
{
  cJSON * profiles = cJSON_CreateObject();
  cJSON * profileFile = cJSON_CreateObject();
  cJSON_AddItemToObject(profileFile, "profiles", profiles);
  return profileFile;
}

void profiles_save(char * rootMinecraft, cJSON * profiles)
{
  _mkdir(rootMinecraft);
  char * fullpath = malloc((strlen(rootMinecraft) + 28));
  strcpy(fullpath, rootMinecraft);
  strcat(fullpath, "launcher_profiles.json");
  json_save_file(profiles, fullpath);
}
