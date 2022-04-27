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
#include <unistd.h>
#include "lib/utils/argumentsParser.h"

int main(int argc, char * argv[])
{
	// init variable
	CmdArg args = parse_arguments(argc, argv);
	char *version = args.version;
	char *root = "/home/coni/.minecraft/";
	char rootBinary[(strlen(root) + 5)];
	char rootVersion[strlen(root) + 10];
	char rootAssets[strlen(root) + 8];
	char rootLibraries[strlen(root) + 11];
	char rootRuntime[strlen(root) + 9];
	
	strcpy(rootRuntime, root);
	strcat(rootRuntime, "runtime/");

	strcpy(rootVersion, root);
	strcat(rootVersion, "versions/");


	strcpy(rootLibraries, root);
	strcat(rootLibraries, "libraries/");

	strcpy(rootBinary, root);
	strcat(rootBinary, "bin/");

	strcpy(rootAssets, root);
	strcat(rootAssets, "assets/");

	CURL *curlSession = curl_easy_init();
  cJSON *versionsManifest = NULL;
	cJSON *versionManifest = NULL;

	// Version Manifest
	printf("Downloading all versions manifest\n");
  versionsManifest = MinecraftManifest_get_all_versions_manifest(rootVersion, curlSession);

	// Download version json from the version manifest
	printf("Downloading %s manifest\n", version);
  versionManifest = MinecraftManifest_get_version_manifest(&versionsManifest, version, rootVersion, curlSession);

	// download assets
	printf("Downloading assets\n");
	MinecraftManifest_download_assets(versionManifest, rootAssets, curlSession);

	// Getting Classpath from the json version
	printf("Downloading LWJGL\n");
	char * lwjglVersion = MinecraftManifest_get_lwjgl_version(versionManifest);
	char * lwjglPath = download_lwjgl(lwjglVersion, rootBinary, curlSession);

	printf("Downloading Libraries\n");
	char *classpath = MinecraftManifest_download_libraries(&versionManifest, rootLibraries, curlSession);

	printf("Downloading Client\n");
	char *mainJar =	MinecraftManifest_download_client(versionManifest, version, rootVersion, curlSession);

	mainJar = realloc(mainJar, (strlen(classpath) + 1 + strlen(mainJar)) * sizeof(char*));
	strcat(mainJar,":");
	strcat(mainJar, classpath);
	free(classpath);
	classpath = mainJar;

	// Getting Mainclass
	char *mainclass = MinecraftManifest_get_mainclass(versionManifest);

	// Get Argument
	JvmArgs jvmArgs = MinecraftManifest_initialize_jvm_arguments();
	jvmArgs.classpath = classpath;
	jvmArgs.natives_directory = lwjglPath;

	GameArgs gameArgs = MinecraftManifest_initialize_game_arguments();
	gameArgs.version_name = version;
	gameArgs.assets_index_name = MinecraftManifest_get_asset_index(versionManifest);

	char * gameArguments = getGameArguments(versionManifest, gameArgs);
	char * javaArguments = getJavaArguments(versionManifest, jvmArgs);

	printf("Downloading Java Runtime\n");
	char * javaPath = MinecraftManifest_download_jre(versionManifest, rootRuntime, curlSession);
	javaPath = realloc(javaPath, (strlen(javaPath) + 10));
	strcat(javaPath, "bin/java");
	
	printf("creating command\n");
	// Create Launch Command
	char * launchCommand = malloc(sizeof(char*) * (strlen(javaPath)  + strlen(javaArguments) + strlen(mainclass) + strlen(gameArguments) + 3));
	strcpy(launchCommand, javaPath);
	strcat(launchCommand, " ");
	strcat(launchCommand, javaArguments);
	strcat(launchCommand, " ");
	strcat(launchCommand, mainclass);
	strcat(launchCommand, " ");
	strcat(launchCommand, gameArguments);

	printf("startgingeg eminecraft");
	chdir(root);
	system(launchCommand);
	printf(launchCommand);
	return 0;
}

