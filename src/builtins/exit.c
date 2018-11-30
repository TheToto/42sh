/**
 * \file exit.c
 * Execute the exit builtin
 * \authors sabrina.meng
 * \version 0.8
 * \date 30-11-2018
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "builtins.h"

int exec_exit(char *str)
{
    size_t n = get_args(str);
    char *cmd = strdup(str);
    char *arg = strtok(cmd, " ");
    arg = strtok(NULL, " ");
    int val;
    if (!n)
        val = 0; //TO FIX : has to be the last command return value
    else
    {
        val = atoi(arg) % 255;
        if (!val && strcmp(arg, "0"))
            val = 2;
        else if (n != 1)
        {
            warnx("exit : too many arguments");
            free(cmd);
            return 0;
        }
    }
    free(cmd);
    exit(val);
}
