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
#include "lib/minecraft/client.h"
#include "lib/minecraft/arguments.h"
#include "lib/minecraft/assets.h"
#include "lib/utils/string.h"
#include "lib/java/java.h"
#include "lib/lwjgl/lwjgl.h"
#include "lib/utils/arch/x64.h"

int main(int argc, char * argv[])
{
	// init variable
	char *version = "1.18.2";
	/* printf("%s -> ", version); */

	char *root = "/home/coni/.minecraft/";
	char *rootBinary = malloc((strlen(root) + 5)*sizeof(char *));
	char *rootVersion = malloc((strlen(root) + 10)*sizeof(char*));
	char *rootAssets = malloc((strlen(root) + 8)*sizeof(char*));
	char *rootLibraries = malloc((strlen(root) + 10)*sizeof(char*));

	strcpy(rootVersion, root);
	strcpy(rootAssets, root);
	strcpy(rootLibraries, root);
	strcpy(rootBinary, root);
	strcat(rootBinary, "bin/");
	strcat(rootVersion, "versions/");
	strcat(rootAssets, "assets/");
	strcat(rootLibraries, "libraries/");

	CURL *session = curl_easy_init();
  cJSON *versionsManifest = NULL;
	cJSON *versionManifest = NULL;
	char *temp = NULL;

	// Version Manifest
	temp = malloc((strlen(rootVersion) + strlen("version_manifest_v2.json") + 1) * sizeof(char *));
	strcpy(temp, rootVersion);
	strcat(temp, "version_manifest_v2.json");
	printf("Downloading manifest\n");
	Http_Download("https://launchermeta.mojang.com/mc/game/version_manifest_v2.json", temp, session);
  parseJsonFile(temp, &versionsManifest);

	// Download version json from the version manifest
	temp = malloc((strlen(rootVersion) + strlen(version) + strlen(version) + 7) * sizeof(char *));
	strcpy(temp, rootVersion);
	strcat(temp, version);
	strcat(temp, "/");
	strcat(temp, version);
	strcat(temp, ".json");
	printf("Downloading %s manifest\n", version);
  downloadMinecraftVersion(&versionsManifest, version, temp, session);

	// Getting Classpath from the json version
  parseJsonFile(temp, &versionManifest);
	char * lwjglVersion = getLwjglVersion(versionManifest);
	char * lwjglPath = malloc((strlen(rootBinary) + strlen(lwjglVersion) + 2)*sizeof(char*));
	strcpy(lwjglPath, rootBinary);
	strcat(lwjglPath, lwjglVersion);
	strcat(lwjglPath, "/");
	printf("Downloading LWJGL\n");
	downloadLwjgl(lwjglVersion, lwjglPath, session);

	printf("Downloading Libraries\n");
	char *classpath = getClasspath_downloadLibraries(&versionManifest, rootLibraries, session);
	printf("Downloading Client\n");
	char *mainJar =	downloadMainJar(versionManifest, version, rootVersion, session);
	mainJar = realloc(mainJar, (strlen(classpath) + 1 + strlen(mainJar)) * sizeof(char*));
	strcat(mainJar,":");
	strcat(mainJar, classpath);
	free(classpath);
	classpath = mainJar;

	// Getting Mainclass
	char *mainclass = getMainclass(versionManifest);

	// Get Argument
	jvmARGS jvmArgs = initJvmArgs();
	jvmArgs.classpath = classpath;
	jvmArgs.natives_directory = "/home/coni/.minecraft/bin/3.2.2/";

	gameARGS gameArgs = initGameArgs();
	gameArgs.version_name = version;
	gameArgs.assets_index_name = getAssetIndex(versionManifest);

	char * gameArguments = getGameArguments(versionManifest, gameArgs);
	char * javaArguments = getJavaArguments(versionManifest, jvmArgs);
	char runtimePath[strlen(root) + 8];
	strcpy(runtimePath, root);
	strcat(runtimePath, "runtime/");
	printf("Downloading Java Runtime\n");
	char * javaPath = downloadJre(versionManifest, runtimePath, session);
	javaPath = realloc(javaPath, (strlen(javaPath) + 10));
	strcat(javaPath, "bin/java ");
	
		// Create Launch Command
	char * launchCommand = malloc(sizeof(char*) * (strlen(javaPath)  + strlen(javaArguments) + strlen(mainclass) + 1 + strlen(gameArguments) + 1));
	strcpy(launchCommand, javaPath);
	strcat(launchCommand, javaArguments);
	strcat(launchCommand, mainclass);
	strcat(launchCommand, " ");
	strcat(launchCommand, gameArguments);

	printf("%s\n", launchCommand);

	return 0;
}
