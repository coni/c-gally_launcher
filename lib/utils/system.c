#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

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
