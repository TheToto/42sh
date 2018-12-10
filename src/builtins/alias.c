/**
* @file alias.c
* @author louis.holleville
* @date 03-12-2018
* @version 1.0
* @brief Execution of the alias and unalias builtin
*/

#define _DEFAULT_SOURCE

#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "env.h"
#include "queue.h"

static char err_op(char *str)
{
    warnx("alias: %s: invalid option", str);
    warnx("alias: usage: alias [-p] [name[=value]]");
    return -1;
}

static char alias_options(char **str)
{
    char res = 0;
    size_t i = 1;
    for (; str[i]; i++)
    {
        if (str[i][0] == '-')
        {
            if (str[i][1] && str[i][1] == 'p')
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
    return res;
}

static int is_opt(char *str)
{
    return str[0] == '-' && str[1] && str[1] == 'p' && !str[2];
}

static void print_aliases(char flag)
{
    struct aliases *alias = shell.alias;
    struct queue *q = init_queue();
    for (size_t i = 0; i < alias->size; i++)
    {
        size_t size = strlen(alias->names[i]) + strlen(alias->values[i]) + 20;
        char *tmp = calloc(size, sizeof(char));
        if (flag)
            sprintf(tmp, "alias %s=\'%s\'\n", alias->names[i],
                    alias->values[i]);
        else
            sprintf(tmp, "%s=\'%s\'\n", alias->names[i],
                    alias->values[i]);
        push_queue(q, tmp);
        free(tmp);
    }
    sort_queue_alias(q);
    for (size_t i = 0; i < q->size; i++)
        printf("%s", q->queue[i]);
    destroy_queue(q);
}

static void my_split(char *str, char *name, char *value)
{
    if (!name)
        err(1, "cannot calloc name in my_split in aliases execution");
    if (!value)
        err(1, "cannot calloc value in my_split in aliases execution");
    size_t i = 0;
    for (; str[i] && str[i] != '='; i++)
        name[i] = str[i];
    value[0] = str[i] ? 0 : 1;
    if (str[i] && !str[i + 1])
        value[0] = 2;
    i = str[i] ? i + 1 : i;
    for (size_t j = 0; str[i]; i++, j++)
        value[j] = str[i];
}

static int look_for(char *name, char flag)
{
    char *res = get_alias(shell.alias, name);
    if (!res)
    {
        warnx("alias: %s: not found", name);
        return 1;
    }
    else if (flag)
        printf("alias %s=\'%s\'\n", name, res);
    else
        printf("%s=\'%s\'\n", name, res);
    return 0;
}

static void assign(char *name, char *value)
{
    for (size_t i = 0; name[i]; i++)
    {
        if (name[i] == '\\' || name[i] == '$' || name[i] == '`'
                || name[i] == '=')
        {
            warnx("alias: %s: invalid alias name", name);
            return;
        }
    }
    add_alias(shell.alias, name, value);
}

int exec_alias(char **str)
{
    int res = 0;
    char flag = alias_options(str);
    if (flag == -1)
        return 2;
    if (flag == 1)
        print_aliases(1);
    else if (!str[1])
        print_aliases(0);
    size_t i = 1;
    while (str[i] && is_opt(str[i]))
        i++;
    for (; str[i]; i++)
    {
        size_t len = strlen(str[i]) + 2;
        char *name = calloc(len, sizeof(char));
        char *value = calloc(len, sizeof(char));
        my_split(str[i], name, value);
        if (value[0] == 1)
            res |= look_for(name, flag);
        else if (value[0] == 2)
            assign(name, "");
        else
            assign(name, value);
        free(name);
        free(value);
    }
    return res;
}
