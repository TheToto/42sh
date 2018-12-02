/**
*   @file export.c
*   @author louis.holleville
*   @version 0.8
*   @date 02/12/2018
*   @brief Execution of the export builtin
*/
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include "shell.h"
#include "builtins.h"
#include "env.h"

static char err_param(char *str)
{
    warnx("'%s' is not a valid identifier", str);
    warnx("usage: export[-n] [name[=value] ...] or export -p");
    return -1;
}

static char name_ok(char *str)
{
    size_t len = 0;
    size_t digits = 0;
    for (; str[len] && str[len] != '='; len++)
    {
        if (str[len] <= '9' && str[len] >= '0')
            digits++;
        else if ((str[len] >= 'A' && str[len] <= 'Z') || (str[len] >= 'a'
                    && str[len] <= 'z') || str[len] == '_')
                continue;
        else
            return 0;
    }
    if (digits == len)
        return 0;
    if (len >= PATH_MAX)
        return 0;
    return 1;
}

static char treat_export(char **str, char *p, char *n)
{
    char res = 0;
    size_t i = 1;
    for (; str[i]; i++)
    {
        if (str[i][0] == '-')
        {
            if (!str[i][2])
            {
                if (str[i][1] == 'p')
                {
                    *p = 1;
                    res += 1;
                }
                else if (str[i][2] == 'n')
                    *n = 1;
                else
                {
                    return err_param(str[i]);
                }
            }
            else
            {
                return err_param(str[i]);
            }
        }
        else if (name_ok(str[i]))
            res += 1;
        else
        {
            warnx("incorrect export: %s", str[i]);
            return -1;
        }
    }
    if (i == 1)
        *p = 1;
    return res;
}

static int handle_p(char flags)
{
    if (flags > 1)
        return 0;
    for (size_t i = 0; i < shell.var->size; i++)
    {
        printf("%s", shell.var->lib[i]->name);
        if (shell.var->lib[i]->exported <= 1)
            printf("=\"%s\"\n", shell.var->lib[i]->value);
        else
            printf("\n");
    }
    return 0;
}

static int handle_n(char **str)
{
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i][0] != '-')
            del_var(shell.var, str[i]);
    }
    return 0;
}

static void my_split(char *str, char *name, char *value)
{
    size_t i = 0;
    for (; str[i] && str[i] != '='; i++)
        name[i] = str[i];
    i += str[i] == '=';
    for (size_t j = 0; str[i]; i++, j++)
        value[j] = str[i];
}

static int handle_export(char **str)
{
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i][0] != '-')
        {
            char name[PATH_MAX] =
            {
                0
            };
            char value[PATH_MAX] =
            {
                0
            };
            my_split(str[i], name, value);
            if (*value)
                add_var(shell.var, name, value, 1);
            else
                add_var(shell.var, name, value, 2);
        }
    }
    return 0;
}

int exec_export(char **str)
{
    char p = 0;
    char n = 0;
    char flags = treat_export(str, &p, &n);
    if (flags == -2)
        return 2;
    else if (flags == -1)
        return 1;
    if (p)
        return handle_p(flags);
    if (n)
        return handle_n(str);
    return handle_export(str);
}
