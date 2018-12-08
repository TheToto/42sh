/**
* @file unalias.c
* @author louis.holleville
* @date 03-12-2018
* @version 0.9
* Execution of the builtin unalias
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "env.h"

static char err_op(char *str)
{
    warnx("unalias: %s: invalid option", str);
    warnx("unalias: usage: unalias [-a] name [name ]");
    return -1;
}

static char unalias_options(char **str)
{
    char res = str[1] ? 0 : -1;
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i][0] == '-')
        {
            if (str[i][1] && str[i][1] == 'a')
            {
                if (!str[i][2])
                    res = 1;
                else
                    return err_op(str[i]);
            }
            else
                return err_op(str[i]);
        }
        else
            break;

    }
    if (res == -1)
        warnx("unalias: usage: unalias [-a] name [name]");
    return res;
}

static int remove_all(void)
{
    struct aliases *alias = shell.alias;
    for (size_t i = 0; i < alias->size; i++)
    {
        free(alias->names[i]);
        free(alias->values[i]);
        alias->names[i] = 0;
        alias->values[i] = 0;
    }
    alias->size = 0;
    return 0;
}

int exec_unalias(char **str)
{
    char flag = unalias_options(str);
    if (flag == -1)
        return 2;
    if (flag == 1)
    {
        return remove_all();
    }
    int res = 0;
    for (size_t i = 1; str[i]; i++)
    {
        if (remove_alias(shell.alias, str[i]))
        {
            res = 1;
            warnx("unalias: %s: not found", str[i]);
        }
    }
    return res;
}
