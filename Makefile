MAINFILE = main.c
CFLAG = -lcurl
LIB = lib/cJSON/cJSON.c lib/utils/http.c
OUTPUT = bin/out

all:
	gcc $(LIB) $(MAINFILE) $(CFLAG) -o $(OUTPUT)

run:
	./$(OUTPUT)
