/**
 * \file shopt.c
 * Execute the shopt builtin
 * \authors sabrina.meng
 * \version 0.8
 * \date 29-11-2018
**/

#include <string.h>
#include <err.h>
#include <stdio.h>

#include "builtins.h"
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

static int is_option(char *opt)
{
    if (!opt)
        return 0;
    if (!strcmp(opt, "-u"))
        return 1;
    else if (!strcmp(opt, "-s"))
        return 2;
    else if (opt[0] == '-')
    {
        warnx("Invalid shell option name");
        return -1;
    }
    return 0;

}

static void shopt_option(int opt, enum shopt shopt)
{
    if (opt == 1)
    {
        if (shopt == NO)
        {
            for (size_t j = 0; j < NB_SHOPT; j++)
                shell.shopt_states[j] = 0;
        }
        else
            shell.shopt_states[shopt - 2] = 0;
    }
    else if (opt == 2)
    {
        if (shopt == NO)
        {
            for (size_t j = 0; j < NB_SHOPT; j++)
                shell.shopt_states[j] = 1;
        }
        else
            shell.shopt_states[shopt - 2] = 1;
    }
}

int shopt_exec(char **str)
{
    size_t n = get_args(str);
    char *arg = str[1];
    int opt = is_option(arg);
    if (!n)
        print_shopt(1, NO);
    else if (opt == 1 || opt == 2)
    {
        enum shopt shopt = get_shopt(str[2]);
        if (shopt == OTHER)
            err_shopt();
        shopt_option(opt, shopt);
    }
    else if (!opt)
    {
        enum shopt shopt;
        for (size_t i = 1; str[i]; i++)
        {
            shopt = get_shopt(str[i]);
            printf("%s         %s\n", str[i],
                    shell.shopt_states[shopt - 2] ? "on" : "off");
        }
    }
    return 0;
}
