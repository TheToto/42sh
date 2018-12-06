/**
*   @file export.c
*   @author louis.holleville
*   @version 0.8
*   @date 02/12/2018
*   @brief Execution of the export builtin
*/
#define _DEFAULT_SOURCE
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "builtins.h"
#include "env.h"
#include "queue.h"

extern char **environ;

static char err_param(char *str)
{
    warnx("'%s' is not a valid identifier", str);
    warnx("usage: export[-n] [name[=value] ...] or export -p");
    return -1;
}

static void *my_realloc(void *p, size_t *size)
{
    char *ptr = realloc(p, 2 * *size);
    if (!ptr)
        err(1, "cannot realloc in my_realloc");
    p = ptr;
    *size *= 2;
    return p;
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
                else if (str[i][1] == 'n')
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
    struct queue *q = init_queue();
    for (size_t i = 0; environ[i]; i++)
    {
        char *name = strdup(environ[i]);
        char *value;
        size_t i = 0;
        for (; name[i] && name[i] != '='; i++);
        if (name[i])
        {
            value = name + i + 1;
            name[i] = 0;
        }
        size_t size = strlen(name) + strlen(value) + 20;
        char *tmp = calloc(size, sizeof(char));
        sprintf(tmp, "export %s=\"%s\"\n", name, value);
        push_queue(q, tmp);
        free(name);
        free(tmp);
    }
    sort_queue_alias(q);
    for (size_t i = 0; i < q->size; i++)
        printf("%s", q->queue[i]);
    destroy_queue(q);
    return 0;
}

static int handle_n(char **str)
{
    size_t count = 0;
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i][0] != '-')
        {
            count++;
            char *var = get_var(shell.var, str[i]);
            if (var)
                add_var(shell.var, str[i], var, 3);
        }
    }
    if (count == 0)
        return handle_p(0);
    return 0;
}

static void my_split(char *str, char *name, char *value)
{
    size_t i = 0;
    size_t j = 0;
    size_t size = 255;
    for (; str[i] && str[i] != '='; i++)
    {
        name[i] = str[i];
        if (i >= size)
            name = my_realloc(name, &size);
    }
    name[i] = '\0';
    i += str[i] == '=';
    size = 255;
    for (; str[i]; i++, j++)
    {
        value[j] = str[i];
        if (i >= size)
            name = my_realloc(name, &size);
    }
    value[j] = '\0';
}

static int handle_export(char **str)
{
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i][0] != '-')
        {
            char *name = calloc(255, sizeof(char));
            char *value = calloc(255, sizeof(char));
            my_split(str[i], name, value);
            if (*value)
                add_var(shell.var, name, value, 1);
            else
                add_var(shell.var, name, value, 2);
            free(name);
            free(value);
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
