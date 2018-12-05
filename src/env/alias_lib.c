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
#include "shell.h"

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
        free(alias->values[pos]);
        alias->values[pos] = strdup(value);
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

static char **my_realloc(char **ptr, size_t *size)
{
    char **new = realloc(ptr, 2 * sizeof(char*) * (*size));
    if (!new)
        err(1, "replace_aliases: cannot realloc char**");
    ptr = new;
    *size *= 2;
    return new;
}

static char *char_realloc(char *ptr, size_t *size)
{
    char *new = realloc(ptr, 2 * sizeof(char) * (*size));
    if (!new)
        err(1, "replace_aliases: cannot realloc char*");
    ptr = new;
    *size *= 2;
    return new;
}

static void expand_scmd(struct ast_node_scmd *node, char **tab, size_t size)
{
    if (size == 0)
    {
        free(node->elements[0]);
        for (size_t i = 0; i < node->elt_size - 1; i++)
        {
            node->elements[i] = node->elements[i + 1];
        }
        node->elt_size -= 1;
        node->elements[size] = 0;
    }
    else if (size == 1)
    {
        free(node->elements[0]);
        node->elements[0] = tab[0];
    }
    else
    {
        node->elt_size += size;

        if (node->elt_size >= node->elt_capacity)
        {
            size_t n_capacity = node->elt_size + 1;
            node->elements = my_realloc(node->elements,&n_capacity);
            node->elt_capacity = n_capacity;
        }
        free(node->elements[0]);
        node->elements[0] = NULL;
        for (size_t i = node->elt_size - 1; i > size; i--)
            node->elements[i - 1] = node->elements[i - size];//shifting args

        for (size_t i = 0; i < size; i++)//inserting new args
            node->elements[i] = tab[i];
        node->elt_size -= 1;
    }
    free(tab);
}

void replace_aliases(struct ast_node_scmd *node)
{
    if (node->elt_size == 0)
        return;
    char **tab = calloc(8, sizeof(char*));
    if (!tab)
        err(1, "replace_aliases: cannot calloc array of char*");
    size_t c_size = 0;
    size_t c_capacity = 8;
    char *expand = get_alias(shell.alias, node->elements[0]);
    if (!expand)
    {
        free(tab);
        return;
    }
    char *target = calloc(8, sizeof(char));
    size_t t_capacity = 8;
    size_t t_size = 0;
    if (!target)
        err(1, "replace_aliases: cannot calloc char*");
    size_t j = 0;
    for (size_t i = 0; expand[i]; i++)
    {
        if (expand[i] != ' ')
        {
            target[j] = expand[i];
            t_size++;
            if (t_size == t_capacity)
                target = char_realloc(target, &t_capacity);
            j++;
        }
        else
        {
            tab[c_size] = strdup(target);
            c_size++;
            free(target);
            target = calloc(8, sizeof(char));
            t_size = 0;
            t_capacity = 8;
            j = 0;
            if (c_size == c_capacity)
                tab = my_realloc(tab, &c_capacity);
            while (expand[i + 1] && expand[i + 1] == ' ')
                i++;
        }
    }
    if (expand[0])
    {
        tab[c_size] = strdup(target);
        free(target);
        c_size++;
    }
    expand_scmd(node, tab, c_size);
}
