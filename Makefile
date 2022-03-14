MAINFILE = main.c
LIB = lib/cJSON/cJSON.c
OUTPUT = program.bin

all:
	gcc $(LIB) $(MAINFILE) -o $(OUTPUT)

run:
	./$(OUTPUT)
