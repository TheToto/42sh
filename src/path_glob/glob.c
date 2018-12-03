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

void explore_dir(char *dir, char *pattern)
{
    DIR *mydir = opendir(dir);
    struct dirent *myfile;
    while((myfile = readdir(mydir)) != NULL)
    {
        printf("%s\n", myfile->d_name);
    }
    closedir(mydir);
}

int main(int argc, char* argv[])
{
    char *path = argv[1];
    char dir[PATH_MAX]
    sprintf(path, "%[^/]/$[\n]", );
    explore_dir("/");
}

