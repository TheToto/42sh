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
#include "env.h"
#include "shell.h"

static int cd_noarg(void)
{
    char *path = get_var(shell.var, "HOME");
    if (path && path[0])
    {
        add_var(shell.var, "OLDPWD", get_var(shell.var, "PWD"), 1);
        if (chdir(path) == -1)
        {
            warnx("cd : cannot change directory");
            return 1;
        }
        char *curdir = get_current_dir_name();
        add_var(shell.var, "PWD", curdir, 1);
        free(curdir);
    }
    return 0;
}

static int cd_arg(char *arg)
{
    if (!strcmp(arg, "-"))
    {
        char *tmp = strdup(get_var(shell.var, "PWD"));
        add_var(shell.var, "PWD", get_var(shell.var, "OLDPWD"), 1);
        add_var(shell.var, "OLDPWD", tmp, 1);
        free(tmp);
        if (chdir(get_var(shell.var, "PWD")) == -1)
        {
            warnx("cd : cannot change directory");
            return 1;
        }
    }
    else
    {
        add_var(shell.var, "OLDPWD", get_var(shell.var, "PWD"), 1);
        if (chdir(arg) == -1)
        {
            warnx("cd : cannot change directory");
            return 1;
        }
        char *curdir = get_current_dir_name();
        add_var(shell.var, "PWD", curdir, 1);
        free(curdir);
    }
    return 0;
}

int changedir(char **str)
{
    size_t n = get_args(str);
    if (n > 1)
    {
        warnx("cd : too many arguments");
        return 1;
    }
    char *arg = str[1];
    if (!get_var(shell.var, "OLDPWD"))
        add_var(shell.var, "OLDPWD", get_var(shell.var, "PWD"), 1);
    if (!arg)
        return cd_noarg();
    return cd_arg(arg);
}
