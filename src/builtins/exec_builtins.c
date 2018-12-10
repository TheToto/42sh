/**
* \file exec_builtins.c
* \brief Execute the builtins
* \authors sabrina.meng
* \version 1.0
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
    if (!strcmp(str, "cd"))
        return CD;
    if (!strcmp(str, "shopt"))
        return SHOPT;
    if (!strcmp(str, "export"))
        return EXPORT;
    if (!strcmp(str, "alias"))
        return ALIAS;
    if (!strcmp(str, "unalias"))
        return UNALIAS;
    if (!strcmp(str, "echo"))
        return ECHO;
    if (!strcmp(str, "continue"))
        return CONTINUE;
    if (!strcmp(str, "break"))
        return BREAK;
    if (!strcmp(str, "source") || !strcmp(str, "."))
        return SOURCE;
    if (!strcmp(str, "history"))
        return HISTORY;
    if (!strcmp(str, "read"))
        return READ;
    return ANY;
}

size_t get_args(char **str)
{
    size_t n = 0;
    if (!*str)
        return 0;
    for (; str[n]; n++)
        continue;
    return n - 1;
}

int is_zero(char *s)
{
    for (size_t i = 0; s[i]; i++)
    {
        if (s[i] != '0')
            return 0;
    }
    return 1;
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
        return exec_continue(str);
    case BREAK:
        return exec_break(str);
    case SOURCE:
        return source(str);
    case HISTORY:
        return exec_history(str);
    case READ:
        return exec_read(str);
    default:
        break;
    }
    return res;
}
