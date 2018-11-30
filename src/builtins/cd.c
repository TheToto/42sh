/**
 * \file cd.c
 * Execute the cd builtin
 * \author sabrina.meng
 * \version 0.8
 * \date 30-11-2018
**/

#define _GNU_SOURCE

#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"

int changedir(char **str)
{
    size_t n = get_args(str);
    if (n > 1)
    {
        warnx("cd : too many arguments");
        return 1;
    }
    char *arg = str[1];
    char prevdir[PATH_MAX];
    if (!arg || strcmp(arg, "-"))
        getcwd(prevdir, sizeof(prevdir));
    if (!arg)
    {
        if (chdir(getenv("HOME")) == -1)
        {
            warnx("cd : cannot change directory");
            return 1;
        }
    }
    else if (!strcmp(arg, "-"))
    {
        char *tmp = strdup(prevdir);
        getcwd(prevdir, sizeof(prevdir));
        if (chdir(tmp) == -1)
        {
            warnx("cd : cannot change directory");
            free(tmp);
            return 1;
        }
        free(tmp);
    }
    else
    {
        if (chdir(arg) == -1)
        {
            warnx("cd : cannot change directory");
            return 1;
        }
    }
    return 0;
}
