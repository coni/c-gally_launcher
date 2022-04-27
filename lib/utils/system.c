#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void system_exec(const char *command)
{
    system(command);
}

void _mkdir(const char *dir) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    *strrchr(tmp, '/') = 0;
    for (p = tmp + 1; *p; p++)
    {    
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU);
}

void system_makeExec(const char *file)
{
    char command[strlen(file) + 10];
    strcpy(command, "chmod +x ");
    strcat(command, file);
    system_exec(command);
}

void system_error(int code, char * string)
{
    fprintf(stderr, string);
    exit(code);
}
