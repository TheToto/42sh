/**
* \file shopt.c
* \brief Execute the shopt builtin
* \authors sabrina.meng
* \version 1.0
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
        return -1;
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

static int print_on_off(int state, int print)
{
    if (!print)
        return 0;
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
    add_var(shell.var, "SHELLOPTS", str, 0);
    free(str);
}

static int change_shopt(char *str, int res, int opt)
{
    enum shopt shopt = get_shopt(str);
    if (shopt == OTHER)
    {
        warnx("shopt %s: invalid shell option name", str);
        res = 1;
    }
    else if (opt == 1 || opt == 2)
        shell.shopt_states[shopt] = opt - 1;
    else if (!opt || opt == 3)
    {
        if (opt != 3)
        {
            char *s = get_shopt_str(shopt);
            int len = 15 - strlen(s);
            printf("%s%*s\t%s\n", s, len, "",
                    shell.shopt_states[shopt] ? "on" : "off");
        }
        if (!shell.shopt_states[shopt])
            res = 1;
    }
    return res;
}

static int set_unset(char *str, int opt, int res, int print)
{
    char *msg = "shopt: cannot set and unset shell options simultaneously";
    if (!str)
        print_on_off(opt - 1, print);
    if (opt == 1)
    {
        int next = is_option(str);
        if (next == 2)
        {
            warnx("%s", msg);
            res = 1;
        }
        else if (next)
            res = print_on_off(opt - 1, print);
    }
    else if (opt == 2)
    {
        int next = is_option(str);
        if (next == 1)
        {
            warnx("%s", msg);
            res = 1;
        }
        else if (next)
            res = print_on_off(opt - 1, print);
    }
    return res;
}

static int check_opt(char **str, int *print, int res)
{
    for (size_t i = 1; str[i]; i++)
    {
        int opt = is_option(str[i]);
        if (!opt)
            break;
        if (opt == -1)
        {
            warnx("shopt: %s: invalid option", str[i]);
            return 2;
        }
        if (opt == 3)
            *print = 0;
    }
    return res;
}

int shopt_exec(char **str)
{
    int opt = 0;
    int in_opt = 1;
    int res = 0;
    int print = 1;
    if (!str[1])
    {
        print_shopt(1, NO);
        return 0;
    }
    res = check_opt(str, &print, res);
    for (size_t i = 1; str[i]; i++)
    {
        if (in_opt > 0)
            in_opt = is_option(str[i]);
        if (in_opt)
        {
            opt = in_opt;
            res = set_unset(str[i + 1], opt, res, print);
        }
        else
            res = change_shopt(str[i], res, opt);
    }
    return res;
}
