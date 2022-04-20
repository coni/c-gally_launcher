MAINFILE = main.c
CFLAG = -lcurl
LIB = lib/lwjgl/x86_64/lwjgl.c lib/java/x86_64/java.c lib/utils/string.c lib/minecraft/assets.c lib/minecraft/arguments.c lib/minecraft/client.c lib/minecraft/classpath.c lib/cJSON/cJSON.c lib/utils/http.c lib/utils/system.c lib/utils/json.c lib/minecraft/versionsManifest.c lib/minecraft/mainclass.c
OUTPUT = bin/gl

debug:
	gcc $(LIB) -g $(MAINFILE) $(CFLAG) -o $(OUTPUT)

run:
	./$(OUTPUT)

all:
	gcc $(LIB) $(MAINFILE) $(CFLAG) -o $(OUTPUT)

