/**
 * \file shopt.c
 * Execute the shopt builtin
 * \authors sabrina.meng
 * \version 0.8
 * \date 29-11-2018
**/

#include <string.h>
#include <err.h>

#include "shopt.h"
#include "shell.h"

enum shopt get_shopt(char *arg)
{
    if (!arg)
        return NO;
    if (!strcmp(arg, "ast_print"))
        return ASTPRINT;
    if (!strcmp(arg, "dotglob"))
        return DOTGLOB;
    if (!strcmp(arg, "expand_aliases"))
        return EXP_ALIAS;
    if (!strcmp(arg, "extglob"))
        return EXTGLOB;
    if (!strcmp(arg, "nocaseglob"))
        return NOCASEGLOB;
    if (!strcmp(arg, "nullglob"))
        return NULLGLOB;
    if (!strcmp(arg, "sourcepath"))
        return SRCPATH;
    if (!strcmp(arg, "xpg_echo"))
        return XPGECHO;
    return OTHER;
}

int shopt_exec(char *str)
{
    char *arg = strtok(str, " ");
    arg = strtok(NULL, " ");
    char *shopt = strtok(NULL, " ");
    enum shopt i = get_shopt(shopt);
    if (!arg)
        print_shopt(1, i);
    else if (i == OTHER)
        err_shopt();
    else if (!strcmp(arg, "-u"))
    {
        if (i == NO)
        {
            for (size_t j = 0; j < NB_SHOPT; j++)
                shell.shopt_states[j] = 0;
        }
        else
            shell.shopt_states[i - 2] = 0;
    }
    else if (!strcmp(arg, "-s"))
    {
        if (i == NO)
        {
            for (size_t j = 0; j < NB_SHOPT; j++)
                shell.shopt_states[j] = 1;
        }
        else
            shell.shopt_states[i - 2] = 1;
    }
    else
    {
        warnx("Invalid shell option name");
        return 1;
    }
    return 0;
}
