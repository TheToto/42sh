/**
 * \file exec_builtins.c
 * Execute the builtins
 * \authors sabrina.meng
 * \version 0.8
 * \date 29-11-2018
**/

#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "builtins.h"
#include "shopt.h"
#include "shell.h"

static enum builtin get_builtin(char *str)
{
    if (!str)
        return ANY;
    if (!strcmp(str, "exit"))
        return EXIT;
    else if (!strcmp(str, "cd"))
        return CD;
    else if (!strcmp(str, "shopt"))
        return SHOPT;
    else if (!strcmp(str, "export"))
        return EXPORT;
    else if (!strcmp(str, "alias"))
        return ALIAS;
    else if (!strcmp(str, "unalias"))
        return UNALIAS;
    else if (!strcmp(str, "echo"))
        return ECHO;
    else if (!strcmp(str, "continue"))
        return CONTINUE;
    else if (!strcmp(str, "break"))
        return BREAK;
    else if (!strcmp(str, "source"))
        return SOURCE;
    else if (!strcmp(str, "history"))
        return HISTORY;
    return ANY;
}

size_t get_args(char **str)
{
    size_t n = 0;
    for ( ; str[n]; n++)
        continue;
    return n - 1;
}

int exec_builtin(char **str)
{
    char *cmd = *str;
    enum builtin builtin = get_builtin(cmd);
    int res = -1;
    switch (builtin)
    {
    case EXIT:
        return exec_exit(str);
    case CD:
        return changedir(str);
    case SHOPT:
        return shopt_exec(str);
    case EXPORT:
        return exec_export(str);
    case ALIAS:
        return exec_alias(str);
    case UNALIAS:
        return exec_unalias(str);
    case ECHO:
        return echo(str);
    case CONTINUE:
        break;
    case SOURCE:
        break;
    case HISTORY:
        break;
    case ANY:
        break;
    default:
        errx(127, "%s : command not found", str[0]);
    }
    return res;
}
