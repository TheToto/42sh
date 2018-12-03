/**
 * \file var.c
 * \author louis.holleville
 * \version 0.3
 * \date 16-11-2018
 * \brief Management of shell variables
 */

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <builtins.h>
#include "env.h"
#include "ast.h"
#include "ast_destroy.h"
#include "shell.h"

extern char **environ;

static void *my_realloc(void *p, size_t *size)
{
    char *ptr = realloc(p, 2 * *size);
    if (!ptr)
        err(1, "cannot realloc in my_realloc");
    p = ptr;
    *size *= 2;
    return p;
}

static void import_exported(struct variables *var)
{
    for (size_t i = 0; environ[i]; i++)
    {
       size_t size = 255;
       char *name = calloc(255, sizeof(char));
       size_t j = 0;
       for (; environ[i][j] && environ[i][j] != '='; j++)
       {
            name[i] = environ[i][j];
            if (i >= size)
                name = my_realloc(name, &size);
       }
       char *value = calloc(255, sizeof(char));
       size = 255;
       if (environ[i][j])
           j++;

       size_t k = 0;
       for (; environ[i][j]; j++, k++)
       {
           value[k] = environ[i][j];
           if (k >= size)
               value = my_realloc(value, &size);
       }

        value[k] = 0;
        if (value[0])
            add_var(var, name, value, 1);
        else
            add_var(var, name, value, 2);
        free(name);
        free(value);
    }
}

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

void add_var(struct variables *var, char *name, char *value, char exported)
{
    if (!var || !name || !value)
    {
        warnx("cannot add_var (name = %s, value = %s", name, value);
        return;
    }
    size_t pos = 0;
    struct var *cur;// = var->lib[pos];
    int found = 1;
    for (; found && pos < var->size; pos++)
    {
        cur = var->lib[pos];
        if (strcmp(cur->name, name) == 0)
            found = 0;
    }

    if (!found)
    {
        free(cur->value);
        cur->value = strdup(value);
        cur->exported = exported;
        return;
    }
    if (pos == var->capacity)
        expand_var(var);
    struct var *new = malloc(sizeof(struct var));
    if (!var)
        errx(1, "cannot malloc new word in add_var");
    new->name = strdup(name);
    new->exported = exported;
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
        //else
            //free(bultin)?
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
//    if (name[0] != '$') // if no $, return name
//        return name;
//    name++; // skip $
    for (; i < var->size; i++)
    {
        cur = var->lib[i];
        //printf("DEBUG : %s %s\n", name, cur->name);
        if (strcmp(name, cur->name) == 0)
            break;
    }
    if (i == var->size)
        return NULL;
    return cur->value;
}

void assign_prefix(struct variables *var, char *prefix)
{
    char name[256] =
    {
        0
    };
    char value[256] =
    {
        0
    };
    sscanf(prefix, "%[^=]=%s", name, value);
    //fprintf(stderr, "Add var %s : %s\n", name, value);
    //recursive call here for further expansion
    add_var(var, name, value, 0);
}

char **replace_var_scmd(struct variables *var, struct ast_node_scmd *scmd)
{
    char **res = calloc(scmd->elt_size + 1, sizeof(char*));
    size_t j = 0;
    for (size_t i = 0; i < scmd->elt_size; i++, j++)
    {
        if (scmd->elements[i][0] == '$')
        {
            char *value = get_var(var, scmd->elements[i] + 1);
            if (value)
                res[j] = strdup(value);
            else
                j--;
        }
        else
            res[j] = strdup(scmd->elements[i]);
    }
    return res;
}
