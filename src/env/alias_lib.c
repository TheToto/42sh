/**
* @author louis.holleville
* @file alias_lib.c
* @date 03-12-2018
* @version 0.8
* Management of the aliases library
*/

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "env.h"
#include "ast.h"

static void get_pos(struct aliases *alias, char *name, size_t *i)
{
    size_t pos = 0;
    char *tmp;
    for (; pos < alias->size; pos++)
    {
        tmp = alias->names[pos];
        if (strcmp(name, tmp) == 0)
        {
            *i = pos;
            return;
        }
    }
    *i = -1;
}

struct aliases *init_alias(void)
{
    struct aliases *new = calloc(1, sizeof(struct aliases));
    if (!new)
        err(1, "cannot calloc struct aliases at init");
    char **names = calloc(8, sizeof(char*));
    if (!names)
    {
        free(new);
        err(1, "cannot calloc char **names at aliases init");
    }
    char **values = calloc(8, sizeof(char*));
    if (!values)
    {
        free(new);
        free(names);
        err(1, "cannot calloc char **values at aliases init");
    }
    new->names = names;
    new->values = values;
    new->size = 0;
    new->capacity = 8;
    return new;
}

void destroy_alias(struct aliases *alias)
{
    if (!alias)
        return;
    for (size_t i = 0; i < alias->size; i++)
    {
        free(alias->names[i]);
        free(alias->values[i]);
    }
    free(alias->names);
    free(alias->values);
    free(alias);
}

void add_alias(struct aliases *alias, char *name, char *value)
{
    if (!alias || !name)
    {
        warnx("cannot add aliase %s, missing arg", name);
        return;
    }
    size_t pos = 0;
    size_t pos0 = 0;
    get_pos(alias, name, &pos);
    if (pos == (pos0 - 1))
    {
        alias->names[alias->size] = strdup(name);
        alias->values[alias->size] = strdup(value);
        alias->size += 1;
    }
    else
    {
        alias->values[pos] = value;
    }
    if (alias->size == alias->capacity)
    {
        char **new = realloc(alias->names, 2 * sizeof(char*) * alias->capacity);
        if (!new)
        {
            err(1, "cannot realloc new names array in struct aliases");
        }
        alias->names = new;
        new = realloc(alias->values, 2 * sizeof(char*) * alias->capacity);
        if (!new)
        {
            err(1, "cannot realloc new values array in struct aliases");
        }
        alias->values = new;
        alias->capacity *= 2;
    }
}

int remove_alias(struct aliases *alias, char *name)
{
    size_t pos = 0;
    char *tmp;
    for (; pos < alias->size; pos++)
    {
        tmp = alias->names[pos];
        if (strcmp(name, tmp) == 0)
        {
            char *tmp_v = alias->values[pos];
            alias->names[pos] = alias->names[alias->size - 1];
            alias->values[pos] = alias->values[alias->size - 1];
            free(tmp);
            free(tmp_v);
            alias->names[alias->size - 1] = 0;
            alias->values[alias->size - 1] = 0;
            alias->size -= 1;
            return 0;
        }
    }
    return 1;
}

char *get_alias(struct aliases *alias, char *name)
{
    size_t pos = 0;
    char *tmp;
    for (; pos < alias->size; pos++)
    {
        tmp = alias->names[pos];
        if (strcmp(name, tmp) == 0)
            return alias->values[pos];
    }
    return NULL;
}

void replace_aliases(struct ast_node_scmd *node, size_t size)
{
    if (node->elt_size == 0)
        return;
    char **tab = calloc(8, sizeof(char*));
    if (!tab)
        err(1, "replace_aliases: cannot calloc array of char*");
    size_t size = 0;
    size_t capacity = 8;
    char *expand = get_alias(node->elements[0]);
    if (!expand)
        return;
    for (size_t i = 0; expand[i]; i++)
    {
        
    }
}
