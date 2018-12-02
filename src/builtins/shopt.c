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
    else if (!strcmp(opt, "-q"))
        return 3;
    else if (opt[0] == '-')
    {
        warnx("Invalid shell option name");
        return -1;
    }
    return 0;

}

static void shopt_option(char **str, int opt)
{
    enum shopt shopt = get_shopt(str[2]);
    if (shopt == NO && opt != 3)
    {
        for (size_t j = 0; j < NB_SHOPT; j++)
            shell.shopt_states[j] = opt == 1 ? 0 : 1;
    }
    else if (opt != 3)
    {
        for (size_t i = 2; str[i]; i++)
        {
            shopt = get_shopt(str[i]);
            if (shopt == OTHER)
                err_shopt();
            else
                shell.shopt_states[shopt] = opt == 1 ? 0 : 1;
        }
    }
}

int shopt_exec(char **str)
{
    size_t n = get_args(str);
    char *arg = str[1];
    int opt = is_option(arg);
    enum shopt shopt;
    if (!n)
        print_shopt(1, NO);
    else if (opt == -1 || (opt == 3 && n > 1))
        return 1;
    else if (opt > 0)
        shopt_option(str, opt);
    else if (!opt)
    {
        int len;
        for (size_t i = 1; str[i]; i++)
        {
            shopt = get_shopt(str[i]);
            if (shopt == OTHER)
                err_shopt();
            else
            {
                len = 19 - strlen(str[i]);
                printf("%s%*s\n", str[i], len,
                        shell.shopt_states[shopt] ? "on" : "off");
            }
        }
    }
    return 0;
}
