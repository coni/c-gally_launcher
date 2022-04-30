#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "lib/utils/json.h"
#include "lib/cJSON/cJSON.h"
#include "lib/utils/http.h"
#include "lib/utils/system.h"
#include "lib/utils/string.h"
#include "lib/lwjgl.h"
#include <unistd.h>
#include "lib/utils/argumentsParser.h"

#include "lib/manifest/versions.h"
#include "lib/manifest/classpath.h"
#include "lib/manifest/mainclass.h"
#include "lib/manifest/client.h"
#include "lib/manifest/arguments.h"
#include "lib/manifest/assets.h"
#include "lib/manifest/java.h"

#include "lib/profiles/profiles.h"

int main(int argc, char * argv[])
{
	// init variable
	CmdArg args = parse_arguments(argc, argv);

	char *root = "/home/coni/.minecraft/";
	char rootBinary[(strlen(root) + 5)];
	char rootVersion[strlen(root) + 10];
	char rootAssets[strlen(root) + 8];
	char rootLibraries[strlen(root) + 11];
	char rootRuntime[strlen(root) + 9];
	
	char profilesPath[strlen(root) + 28];
	strcpy(profilesPath, root);
	strcat(profilesPath, "launcher_profiles.json");

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
	cJSON * profiles = NULL;

	char * lwjglPath = NULL;
	
	if (system_file_exist(profilesPath) == 0)
	{
		profiles = json_parse_file(profilesPath);
		if (cJSON_GetObjectItemCaseSensitive(profiles, "profiles") == NULL)
		{
			profiles = profiles_initialization();
			profiles_save(root, profiles);
		}
	}
	else
	{
		profiles = profiles_initialization();
		profiles_save(root, profiles);
	}
	
	// Version Manifest
  versionsManifest = MinecraftManifest_get_all_versions_manifest(rootVersion, curlSession);
	
	if (strcmp(args.version_list, "") != 0)
	{
		MinecraftManifest_list_version(rootVersion, versionsManifest, args.version_list);
	}
	else if (args.version != NULL)
	{
		if (MinecraftManifest_version_exist(args.version, rootVersion, versionsManifest) == 1)
			system_error(1, "this version does not exist\n");

		char *version = args.version;
		char *mainclass = NULL;
		char *inherits = args.version;
		char *classpath = malloc(sizeof(char*)+1);
		char * javaPath = NULL;
		char * gameArguments = NULL;
		char * javaArguments = malloc(sizeof(char*) * 2);
		strcpy(javaArguments, "");

		strcpy(classpath, "");
		
		GameArgs gameArgs = MinecraftManifest_initialize_game_arguments();
		JvmArgs jvmArgs = MinecraftManifest_initialize_jvm_arguments();
		
		if (args.username != NULL)
			gameArgs.auth_player_name = args.username;
		gameArgs.version_name = version;
		
		while (inherits != NULL)
		{
			// Download version json from the version manifest
			printf("Downloading %s manifest\n", inherits);
			versionManifest = MinecraftManifest_get_version_manifest(&versionsManifest, inherits, rootVersion, curlSession);


			// download assets
			if (args.assets_ignore == 0)
			{
				printf("Downloading assets\n");
				MinecraftManifest_download_assets(versionManifest, rootAssets, curlSession);
			}

			// Getting Classpath from the json version
			printf("Downloading LWJGL\n");
			char * lwjglVersion = MinecraftManifest_get_lwjgl_version(versionManifest);
			if (lwjglVersion != NULL)
			{
				lwjglPath = download_lwjgl(lwjglVersion, rootBinary, curlSession);
				jvmArgs.natives_directory = lwjglPath;
			}

			printf("Downloading Libraries\n");
			char *libraries = MinecraftManifest_download_libraries(&versionManifest, rootLibraries, curlSession);

			printf("Downloading Client\n");
			char *mainJar =	MinecraftManifest_download_client(versionManifest, inherits, rootVersion, curlSession);

			if (mainJar)
			{
				classpath = realloc(classpath, (strlen(mainJar) + strlen(classpath) + 3));
				if (strcmp(mainJar, "") != 0)
				{
					if (strcmp(classpath, "") == 1)
						strcat(classpath, ":");
					strcat(classpath, mainJar);
					strcat(classpath, ":");
				}
			}

			if (libraries)
			{
				classpath = realloc(classpath, (strlen(libraries) + strlen(classpath) + 2));
				if (strcmp(classpath, "") == 1)
					strcat(classpath, ":");
				strcat(classpath, libraries);
			}

			// Getting Mainclass
			if (strcmp(inherits, version) == 0 || (mainclass == NULL && strcmp(inherits, version) == 1))
				mainclass = MinecraftManifest_get_mainclass(versionManifest);

			char *t = MinecraftManifest_get_asset_index(versionManifest);
			if (t)
				gameArgs.assets_index_name = t;

			printf("Downloading Java Runtime\n");
			javaPath = MinecraftManifest_download_jre(versionManifest, rootRuntime, curlSession);
			if (javaPath)
			{
					javaPath = realloc(javaPath, (strlen(javaPath) + 10));
					strcat(javaPath, "bin/java");
			}

			jvmArgs.classpath = classpath;
			t = getJavaArguments(versionManifest, jvmArgs);
			javaArguments = realloc(javaArguments, (strlen(javaArguments) + strlen(t) + 2));
			if (strcmp(javaArguments, "") == 1)
				strcat(javaArguments, " ");
			strcat(javaArguments, t);

			gameArguments = getGameArguments(versionManifest, gameArgs);

			// get inherits 
			inherits = MinecraftManifest_get_inherit(versionManifest);

		}
		
		
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
	}
	return 0;
}

