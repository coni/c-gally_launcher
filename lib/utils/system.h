#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

void _mkdir(const char *dir);
void system_makeExec(const char *file);
void system_error(int code, char * string);
