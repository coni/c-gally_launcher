#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "lib/utils/json.h"
#include "lib/minecraft/classpath.h"
#include "lib/cJSON/cJSON.h"
#include "lib/utils/http.h"
#include "lib/utils/system.h"
#include "lib/minecraft/versionsManifest.h"
#include "lib/minecraft/mainclass.h"

int main()
{
	CURL *session = curl_easy_init();
  cJSON *versionsManifest = NULL;
	cJSON *versionManifest = NULL;
	char *url, *sha1, *type = NULL;

	Http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", "trash/version_manifest_v2.json", session);
  parseJsonFile("trash/version_manifest_v2.json", &versionsManifest);
  getMinecraftVersion(&versionsManifest,"1.12.2", &url, &sha1, &type);

	Http_Download(url, "trash/1.18.2.json", session);
  parseJsonFile("trash/1.18.2.json", &versionManifest);

	char *classpath = getClasspath_downloadLibraries(&versionManifest, "trash/minecraft/libraries/", session);
	char *mainclass = getMainclass(versionManifest);

	return 0;
}
