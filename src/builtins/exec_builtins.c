/**
* \file exec_builtins.c
* Execute the builtins
* \authors sabrina.meng
* \version 0.8
* \date 29-11-2018
*/

#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "builtins.h"
#include "shopt.h"
#include "shell.h"

static enum builtin get_builtin(char *str)
{
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

int exec_builtin(char *str)
{
    char *cmd = strdup(str);
    char *tmp = strtok(cmd, " ");
    enum builtin builtin = get_builtin(tmp);
    if (builtin == ANY)
        return -1;
    switch (builtin)
    {
    case EXIT:
        exit(0);
    case CD:
        break;
    case SHOPT:
        if (shopt_exec(str) && shell.type != S_PROMPT)
            exit(1);
    case EXPORT:
        break;
    case ALIAS:
        break;
    case UNALIAS:
        break;
    case ECHO:
        break;
    case CONTINUE:
        break;
    case SOURCE:
        break;
    case HISTORY:
        break;
    default:
        errx(127, "%s : command not found", str);
    }
    free(cmd);
    return 0;
}
