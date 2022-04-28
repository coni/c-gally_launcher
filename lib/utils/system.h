#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

void _mkdir(const char *dir);
void system_makeExec(const char *file);
void system_error(int code, char * string);
int system_ls(char * path, int type, char *array[]);
int system_count_files(char * path, int type);
int system_file_exist(char * path);
