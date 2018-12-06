/**
* \file shopt.c
* Execute the shopt builtin
* \authors sabrina.meng
* \version 0.8
* \date 05-12-2018
**/

#include <string.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtins.h"
#include "shopt.h"
#include "shell.h"
#include "env.h"

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

static char *get_shopt_str(enum shopt shopt)
{
    switch (shopt)
    {
    case ASTPRINT:
        return "ast_print";
    case DOTGLOB:
        return "dotglob";
    case EXP_ALIAS:
        return "expand_aliases";
    case EXTGLOB:
        return "extglob";
    case NOCASEGLOB:
        return "nocaseglob";
    case NULLGLOB:
        return "nullglob";
    case XPGECHO:
        return "xpg_echo";
    case SRCPATH:
        return "sourcepath";
    default:
        return NULL;
    }
}

static void print_on_off(int state)
{
    for (size_t i = 0; i < NB_SHOPT; i++)
    {
        if (shell.shopt_states[i] == state)
        {
            char *str = get_shopt_str(i);
            int len = 15 - strlen(str);
            printf("%s%*s\t%s\n", str, len, "",
                    shell.shopt_states[i] ? "on" : "off");
        }
    }
}

static int shopt_option(char **str, int opt)
{
    enum shopt shopt = get_shopt(str[2]);
    if (shopt == NO && opt != 3)
        print_on_off(opt - 1);
    else if (opt != 3)
    {
        for (size_t i = 2; str[i]; i++)
        {
            shopt = get_shopt(str[i]);
            if (shopt == OTHER)
                return err_shopt();
            else
                shell.shopt_states[shopt] = opt == 1 ? 0 : 1;
        }
    }
    return 0;
}

void update_shellopts(void)
{
    char *str = calloc(4096, sizeof(char));
    if (!str)
        warnx("Calloc error in update_shellopts");
    else
    {
        for (size_t i = 0; i < NB_SHOPT; i++)
        {
            if (shell.shopt_states[i])
            {
                if (str[0])
                    strcat(str, ":");
                if (i == ASTPRINT)
                    strcat(str, "ast_print");
                else if (i == DOTGLOB)
                    strcat(str, "dotglob");
                else if (i == EXP_ALIAS)
                    strcat(str, "expand_aliases");
                else if (i == EXTGLOB)
                    strcat(str, "extglob");
                else if (i == NOCASEGLOB)
                    strcat(str, "nocaseglob");
                else if (i == NULLGLOB)
                    strcat(str, "nullglob");
                else if (i == SRCPATH)
                    strcat(str, "sourcepath");
                else if (i == XPGECHO)
                    strcat(str, "xpg_echo");
            }
        }
    }
    add_var(shell.var, "SHELLOPTS", str, shell.shopt_states[EXP_ALIAS]);
    free(str);
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
        return shopt_option(str, opt);
    else if (!opt)
    {
        int len;
        int res = 0;
        for (size_t i = 1; str[i]; i++)
        {
            shopt = get_shopt(str[i]);
            if (shopt == OTHER)
                return err_shopt();
            else
            {
                len = 15 - strlen(str[i]);
                printf("%s%*s\t%s\n", str[i], len, "",
                        shell.shopt_states[shopt] ? "on" : "off");
                if (!shell.shopt_states[shopt])
                    res = 1;
            }
        }
        return res;
    }
    update_shellopts();
    return 0;
}
