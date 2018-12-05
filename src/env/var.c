/**
* \file var.c
* \author louis.holleville
* \version 0.3
* \date 16-11-2018
* \brief Management of shell variables
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "builtins.h"
#include "env.h"
#include "ast.h"
#include "ast_destroy.h"
#include "shell.h"
#include "shopt.h"
#include "quote_lexer.h"
#include "maths.h"
#include "queue.h"

extern char **environ;

static char *itoa(int i, char *buf_nb)
{
    sprintf(buf_nb, "%d", i);
    return buf_nb;
}

static void set_up_reserved(void)
{
    char buf_nb[20] =
    {
        0
    };
    add_var(shell.var, "$", itoa(getpid(), buf_nb), 0);
    add_var(shell.var, "UID", itoa(getuid(), buf_nb), 0);
    char *pwd = get_current_dir_name();
    if (!get_var(shell.var, "OLDPWD"))
        add_var(shell.var, "OLDPWD", pwd, 0);
    free(pwd);
    add_var(shell.var, "RANDOM", "32767", 0);
    add_var(shell.var, "?", "", 0);
    // SHELLOPTS
    add_var(shell.var, "IFS", " \\t\\n", 0);
}

void set_up_var(char *args[])
{
    char buf_nb[20] =
    {
        0
    };
    int nb = 0;
    int size = 0;
    for (; args && args[nb] && nb < 100; nb++)
    {
        add_var(shell.var, itoa(nb, buf_nb), args[nb], 0);
        size += strlen(args[nb]) + 1;
    }
    add_var(shell.var, "#", itoa(nb - 1, buf_nb), 0);
    // $@ $*
    char *star = calloc(size, sizeof(char));
    for (size_t i = 1; args && args[i]; i++)
    {
        strcat(star, args[i]);
        if (args[i + 1])
            strcat(star, " ");
    }
    add_var(shell.var, "*", star, 0);
    add_var(shell.var, "@", star, 0);
    free(star);
    set_up_reserved();
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

static void import_exported(struct variables *var)
{
    for (size_t i = 0; environ[i]; i++)
    {
       size_t size = 255;
       char *name = calloc(255, sizeof(char));
       size_t j = 0;
       for (; environ[i][j] && environ[i][j] != '='; j++)
       {
            name[j] = environ[i][j];
            if (i >= size - 1)
                name = my_realloc(name, &size);
       }
       name[j] = '\0';
       char *value = calloc(255, sizeof(char));
       size = 255;
       if (environ[i][j])
           j++;

       size_t k = 0;
       for (; environ[i][j]; j++, k++)
       {
           value[k] = environ[i][j];
           if (k >= size - 1)
               value = my_realloc(value, &size);
       }

        value[k] = '\0';
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
    if (*name && *name == '(' && *name + 1 == '(')
        return get_maths(name);
    //if (*name && *name == '(')
    //    return exec_subshell(name);


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
    for (size_t i = 0; i < 256 && *prefix != '='; i++, *prefix++)
        name[i] = *prefix;
    prefix++;
    for (size_t i = 0; i < 256 && *prefix != '\0'; i++, *prefix++)
        value[i] = *prefix;
    char *val = remove_quoting(value);
    add_var(var, name, val, 0);
    free(val);
}

char **replace_var_scmd(struct ast_node_scmd *scmd)
{
    if (shell.shopt_states[EXP_ALIAS])
        replace_aliases(scmd);
    struct *res = init_queue();
    size_t j = 0;
    for (size_t i = 0; i < scmd->elt_size; i++, j++)
    {
        remove_quoting(scmd->elements[i], res);
    }
    return dump_queue(res);
}
