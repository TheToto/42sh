/**
* \file var.c
* \author louis.holleville
* \version 1.0
* \date 16-11-2018
* \brief Management of shell variables
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fnmatch.h>

#include "builtins.h"
#include "env.h"
#include "ast.h"
#include "ast_destroy.h"
#include "shell.h"
#include "shopt.h"
#include "quote_lexer.h"
#include "shopt.h"
#include "maths.h"
#include "queue.h"
#include "pathexp.h"
#include "subshell.h"

extern char **environ;

struct variables *init_var(void)
{
    struct variables *new = malloc(sizeof(struct variables));
    if (!new)
    {
        err(1, "cannot malloc in init_var");
        return NULL;
    }
    struct var **lib = calloc(8, sizeof(struct lib*));
    if (!lib)
    {
        err(1, "cannot calloc in init_var");
        free(new);
        return NULL;
    }
    struct func **f_lib = calloc(8, sizeof(struct func*));
    if (!lib)
    {
        err(1, "cannot calloc in init_var");
        free(new);
        return NULL;
    }
    new->lib = lib;
    new->f_lib = f_lib;
    new->f_size = 0;
    new->f_capacity = 8;
    new->size = 0;
    new->capacity = 8;
    shell.var = new;
    import_exported(new);
    set_up_reserved();
    return new;
}

static void expand_var(struct variables *var)
{
    struct var **new = realloc(var->lib, 2 * var->capacity
            * sizeof(struct variables*));
    if (!new)
        errx(1,"cannot realloc in expand_var");
    var->lib = new;
    var->capacity *= 2;
}

static void add_var_not_found(struct var *cur,  char *name, char *value,
        char exported)
{
    if (value != cur->value)
    {
        free(cur->value);
        cur->value = strdup(value);
    }
    if (cur->exported)
        setenv(name, value, 1);
    if (exported == 3)
    {
        unsetenv(name);
        cur->exported = 0;
    }
    if (cur->exported == 2 && exported == 1)
        cur->exported = exported;
    return;
}
void add_var(struct variables *var, char *name, char *value, char exported)
{
    if (!var || !name || !value)
    {
        warnx("cannot add_var (name = %s, value = %s", name, value);
        return;
    }
    size_t pos = 0;
    struct var *cur;
    int found = 1;
    for (; found && pos < var->size; pos++)
    {
        cur = var->lib[pos];
        if (strcmp(cur->name, name) == 0)
            found = 0;
    }

    if (!found)
    {
        add_var_not_found(cur, name, value, exported);
        return;
    }
    if (pos == var->capacity)
        expand_var(var);
    struct var *new = malloc(sizeof(struct var));
    if (!var)
        errx(1, "cannot malloc new word in add_var");
    new->name = strdup(name);
    new->exported = exported;
    if (new->exported)
        setenv(name, value, 1);
    new->value = strdup(value);
    var->size += 1;
    var->lib[pos] = new;
}

void destroy_var(struct variables *var)
{
    if (!var)
    {
        warnx("cannot destroy_var: no lib provided");
        return;
    }
    struct var *cur;
    for (size_t i = 0; i < var->size; i++)
    {
        cur = var->lib[i];
        free(cur->name);
        free(cur->value);
        free(cur);
    }
    free(var->lib);
    struct func *next;
    for (size_t i = 0; i < var->f_size; i++)
    {
        next = var->f_lib[i];
        free(next->name);
        if (next->type == DECLARED)
            destroy_ast(next->value);
        free(next);
    }
    free(var->f_lib);
    free(var);
    shell.var = NULL;
}

void del_var(struct variables *var, char *name)
{
    size_t i = 0;
    size_t last = var->size - 1;
    struct var *cur;
    while (i < var->size)
    {
        cur = var->lib[i];
        if (strcmp(name, cur->name) == 0 && cur->exported >= 1)
        {
            var->lib[i] = var->lib[last];
            var->lib[last] = 0;
            free(cur->name);
            free(cur->value);
            free(cur);
            var->size -= 1;
            break;
        }
        i++;
    }
    unsetenv(name);
}

char *get_var(struct variables *var, char *name)
{
    if (!var || !name)
    {
        warnx("cannot get_var: no var or name provided");
        return NULL;
    }

    struct var *cur;
    size_t i = 0;

    for (; i < var->size; i++)
    {
        cur = var->lib[i];
        if (strcmp(name, cur->name) == 0)
            break;
    }
    if (i == var->size)
        return NULL;
    if (!strcmp(name, "RANDOM"))
        sprintf(cur->value, "%d", rand() % 32767);
    return cur->value;
}
