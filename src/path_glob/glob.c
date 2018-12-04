#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <libgen.h>


char *get_next_path(char *path, char *dir)
{
    while (path && *path && *path != '/')
    {
        *dir = *path;
        dir++;
        path++;
    }
    if (*path)
        path++;
    *dir = '\0';
    return path;
}

void explore_dir(char *cur_path, char *path, char *patern)
{
    DIR *mydir = opendir(cur_path);
    if (!mydir)
        return;
    struct dirent *myfile;
    while((myfile = readdir(mydir)) != NULL)
    {
        //printf("Compare %s:%s / %s\n", myfile->d_name, patern, path);
        if (!fnmatch(patern, myfile->d_name, 0))
        {
            //printf("Enter : %s\n", myfile->d_name);
            char *new_path = strdup(path);
            char *new_patern = calloc(PATH_MAX, sizeof(char));
            char *new_cur_path = calloc(PATH_MAX, sizeof(char));
            char *alt_path = get_next_path(new_path, new_patern);
            if (!strlen(new_patern) && strcmp(myfile->d_name, ".")
                    && strcmp(myfile->d_name, ".."))
                printf("Found : %s\n", myfile->d_name);
            strcat(new_cur_path, cur_path);
            strcat(new_cur_path, "/");
            strcat(new_cur_path, myfile->d_name);
            explore_dir(new_cur_path, alt_path, new_patern);
            free(new_patern);
            free(new_cur_path);
            free(new_path);
        }
    }
    closedir(mydir);
}



int main(int argc, char* argv[])
{
    char *path = argv[1];
    char *cur_path = calloc(PATH_MAX, sizeof(char));
    char *dir = calloc(PATH_MAX, sizeof(char));

    if (*path == '/')
    {
        strcat(cur_path, "/");
        *path++;
    }
    else
    {
        strcat(cur_path, ".");
    }
    path = get_next_path(path, dir);
    explore_dir(cur_path, path, dir);
    free(cur_path);
    free(dir);
}
