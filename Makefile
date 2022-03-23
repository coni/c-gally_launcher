MAINFILE = main.c
CFLAG = -lcurl
LIB = lib/minecraft/client.c lib/minecraft/classpath.c lib/cJSON/cJSON.c lib/utils/http.c lib/utils/system.c lib/utils/json.c lib/minecraft/versionsManifest.c lib/minecraft/mainclass.c
OUTPUT = bin/out

debug:
	gcc $(LIB) -g $(MAINFILE) $(CFLAG) -o $(OUTPUT)

run:
	./$(OUTPUT)

all:
	gcc $(LIB) $(MAINFILE) $(CFLAG) -o $(OUTPUT)

