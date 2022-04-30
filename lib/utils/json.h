#include "../cJSON/cJSON.h"

cJSON * json_parse_file(const char *filename);
void json_save_file(cJSON * json, char * filename);
