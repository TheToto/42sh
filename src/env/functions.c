/**
 * @file functions.c
 * @author louis.holleville
 * @date 25-11-2018
 * @version 0.5
 * Management of functions declaration and builtins
 */

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "env.h"
#include "ast_destroy.h"
#include "ast.h"

static void expand_f_lib(struct variables *var)
{
    struct func **new = realloc(var->f_lib, 2 * sizeof(struct func*)
            * var->f_capacity);
    if (!new)
        err(1, "cannot realloc in expand_f_lib");
    var->f_lib = new;
    var->f_capacity *= 2;
}

int add_func(struct variables *var, char *name, void *value, enum f_type type)
{
    if (!var || !name || !value)
    {
        warnx("cannot add_func (name = %s) in add_func", name);
        return 1;
    }
    size_t pos = 0;
    struct func *cur = var->f_lib[pos];
    int found = 1;
    for (; found && pos < var->size; pos++)
    {
        if (strcmp(cur->name, name) == 0)
            found = 0;
        cur = var->f_lib[pos];
    }

    if (!found)
    {
        if (cur->type == DECLARED)
            destroy_ast(cur->value);
        //else
            //free(builtin)?
        cur->value = value;
        cur->type = type;
        return 0;
    }
    if (pos == var->capacity)
        expand_f_lib(var);
    struct func *new = malloc(sizeof(struct func));
    if (!new)
        err(1, "cannot malloc new word in add_var");
    new->name = strdup(name);
    new->type = type;
    new->value = value;
    var->f_size += 1;
    var->f_lib[pos] = new;
    return 0;
}

void *get_func(struct variables *var, char *name)
{
    if (!var || !name)
    {
        warnx("cannot get_var: no var or name provided");
        return NULL;
    }
    struct func *cur;
    size_t i = 0;
    for (; i < var->f_size; i++)
    {
        cur = var->f_lib[i];
        if (strcmp(name, cur->name) == 0)
            break;
    }
    if (i == var->f_size)
        return NULL;
    return cur->value;
}

