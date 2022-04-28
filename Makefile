MAINFILE = main.c
CFLAG = -lcurl
LIB = lib/utils/argumentsParser.c lib/lwjgl.c lib/manifest/java.c lib/utils/string.c lib/manifest/assets.c lib/manifest/arguments.c lib/manifest/client.c lib/manifest/classpath.c lib/cJSON/cJSON.c lib/utils/http.c lib/utils/system.c lib/utils/json.c lib/manifest/versions.c lib/manifest/mainclass.c arch/x64.c
OUTPUT = bin/gl

debug:
	gcc $(LIB) -g $(MAINFILE) $(CFLAG) -o $(OUTPUT)

run:
	./$(OUTPUT) --version-list downloaded

all:
	gcc $(LIB) $(MAINFILE) $(CFLAG) -o $(OUTPUT)

