#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <unistd.h>

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

int system_file_exist(char * path)
{
  return access( path, F_OK );
}

int system_count_files(char * path, int type)
{
    if (type == 1)
        type = DT_REG;
    else if (type == 2)
        type = DT_DIR;
    DIR *d;
    struct dirent *dir;
    int count = 0;
    d = opendir(path);
    if (d) {
        if (type != 0)
        {
            while ((dir = readdir(d)) != NULL) 
            {
                if(dir-> d_type == type) //
                    count++;
            }
        }
        else
        {
            while ((dir = readdir(d)) != NULL) 
            {
                count++;
            }
        }
        closedir(d);
    }
    return count;
}

void system_ls(char * path, int type, char *array[])
{
    if (type == 1)
        type = DT_REG;
    else if (type == 2)
        type = DT_DIR;
    DIR *d;
    struct dirent *dir;
    int count = 0;
    char * files = NULL;
    d = opendir(path);
    if (d) {
        if (type != 0)
        {
            count = 0;
            while ((dir = readdir(d)) != NULL) 
            {
                if(dir-> d_type == type) //
                {
                    /* strcpy(array[count], dir->d_name); */
                    array[count] = dir->d_name;
                    count++;
                }
            }
        }
        else
        {
            count = 0;
            while ((dir = readdir(d)) != NULL) 
            {
                strcpy(array[count], dir->d_name);
                count++;
            }
        }
        closedir(d);
    }
}
