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
	int a = 0;
	char *version = "1.18.2";
	char *root = "/home/coni/.minecraft/";
	char *rootVersion = malloc((strlen(root) + 10)*sizeof(char*));
	char *rootAssets = malloc((strlen(root) + 8)*sizeof(char*));
	char *rootLibraries = malloc((strlen(root) + 10)*sizeof(char*));

	a = 27270;
	strcpy(rootVersion, root);
	strcpy(rootAssets, root);
	strcpy(rootLibraries, root);
	strcat(rootVersion, "versions/");
	strcat(rootAssets, "assets/");
	strcat(rootLibraries, "libraries/");

	CURL *session = curl_easy_init();
  cJSON *versionsManifest = NULL;
	cJSON *versionManifest = NULL;
	char *url, *sha1, *type = NULL;
	char *temp = NULL;

	temp = malloc((strlen(rootVersion) + strlen("version_manifest_v2.json") + 1) * sizeof(char *));
	strcpy(temp, rootVersion);
	strcat(temp, "version_manifest_v2.json");
	Http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", temp, session);
  parseJsonFile(temp, &versionsManifest);
  getMinecraftVersion(&versionsManifest, version, &url, &sha1, &type);

	temp = malloc((strlen(rootVersion) + strlen(version) + strlen(version) + 7) * sizeof(char *));
	strcpy(temp, rootVersion);
	strcat(temp, version);
	strcat(temp, "/");
	strcat(temp, version);
	strcat(temp, ".json");
	Http_Download(url, temp, session);
  parseJsonFile(temp, &versionManifest);

	char *classpath = getClasspath_downloadLibraries(&versionManifest, rootLibraries, session);
	char *mainclass = getMainclass(versionManifest);

	return 0;
}
