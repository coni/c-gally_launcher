#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

cJSON * json_parse_file(const char *filename)
{ 
	int c;
	int index = 0;
	int size = 2;
  char *content = (char *) malloc(size);
	FILE *fp;

  fp = fopen(filename, "r");
	while ((c = getc(fp)) != EOF)
	{
		if (index >= size-1)
			content = (char *) realloc(content, size++);
		content[index] = c;
		index++;
	}
	fclose(fp);

  return cJSON_Parse(content);
} 
