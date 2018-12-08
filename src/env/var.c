/**
* \file var.c
* \author louis.holleville
* \version 0.8
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
    add_var(shell.var, "OLDPWD", "", 0);
    add_var(shell.var, "PWD", pwd, 0);
    free(pwd);
    add_var(shell.var, "RANDOM", "32767", 0);
    add_var(shell.var, "?", "", 0);
    add_var(shell.var, "PS1", "[\\u@\\h \\W ]$ ", 0);
    add_var(shell.var, "PS2", "> ", 0);

    if (!get_var(shell.var, "HOME"))
        add_var(shell.var, "HOME", getenv("HOME"), 0);
    shell.shopt_states = init_shoptlist();
    update_shellopts();
    add_var(shell.var, "IFS", " \t\n", 0);
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
    char *star = calloc(size + 1, sizeof(char));
    for (size_t i = 1; args && args[0] && args[i]; i++)
    {
        strcat(star, args[i]);
        if (args[i + 1])
            strcat(star, " ");
    }
    add_var(shell.var, "*", star, 0);
    add_var(shell.var, "@", star, 0);

    free(star);
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
        add_var(var, name, value, 1);

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

char *get_sub_and_maths(char *name)
{
    if (*name && !fnmatch("(*(*))", name, FNM_EXTMATCH))
        return get_maths(name);
    else
        return redirect_subshell(name);
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

void assign_prefix(struct variables *var, char *prefix)
{
    char name[2048] =
    {
        0
    };
    char value[2048] =
    {
        0
    };
    for (size_t i = 0; i < 256 && *prefix != '='; i++, *prefix++)
        name[i] = *prefix;
    prefix++;
    for (size_t i = 0; i < 256 && *prefix != '\0'; i++, *prefix++)
        value[i] = *prefix;
    char *concat = concat_quote(value);
    add_var(var, name, concat, 0);
    free(concat);
}

void check_string_at(char *in, struct queue *q)
{
    for (size_t i = 0; in && in[i];)
    {
        if (in[i] == '\'')
        {
            i++;
            while (in[i] && in[i] != '\'')
                i++;
        }
        else if (in[i] == '"')
        {
            i++;
            while (in[i] && in[i] != '"')
            {
                if (in[i] == '$' && in[i + 1] == '@')
                {
                    //si # 0 ou 1 do nothing
                    char *last = get_var(shell.var, "#");
                    if (!last || last[0] == '0' || last[0] == '1')
                    {
                        push_queue(q, in);
                        return;
                    }
                    char *dup = strdup(in);
                    in[i + 1] = '1';
                    in[i + 2] = '"';
                    in[i + 3] = '\0';
                    push_queue(q, in);
                    for (char k = '2'; k < last[0]; k++)
                    {
                        char *toadd = calloc(10, sizeof(char));
                        sprintf(toadd, "\"$%c\"", k);
                        push_queue(q, toadd);
                        free(toadd);
                    }
                    dup[i - 1] = '"';
                    dup[i + 1] = last[0];
                    push_queue(q, &dup[i - 1]);
                    free(dup);
                    return;
                }
                else if (in[i + 1] && in[i] == '\\')
                    i++;
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    push_queue(q, in);
}

char **check_at(char **input)
{
    struct queue *q = init_queue();
    for (size_t i = 0; input[i]; i++)
    {
        check_string_at(input[i], q);
        free(input[i]);
    }
    free(input);
    return dump_queue(q);
}

char **replace_var_scmd(struct ast_node_scmd *scmd)
{
    if (shell.shopt_states[EXP_ALIAS])
        replace_aliases(scmd);
    scmd->elements = check_at(scmd->elements);
    struct queue *qot = init_queue();
    size_t j = 0;
    for (size_t i = 0; scmd->elements[i]; i++, j++)
    {
        remove_quoting(scmd->elements[i], qot);
    }
    struct queue *res = init_queue();
    for (size_t i = 0; i < qot->size; i++)
    {
        struct queue *tmp = expand_path(qot->queue[i]);
        fusion_queue(res, tmp);
    }
    destroy_queue(qot);
    return dump_queue(res);
}

char **replace_var_for(struct ast_node_for *n_for)
{
    n_for->values = check_at(n_for->values);
    struct queue *qot = init_queue();
    size_t j = 0;
    for (size_t i = 0; i < n_for->size; i++, j++)
    {
        remove_quoting(n_for->values[i], qot);
    }
    struct queue *res = init_queue();
    for (size_t i = 0; i < qot->size; i++)
    {
        struct queue *tmp = expand_path(qot->queue[i]);
        fusion_queue(res, tmp);
    }
    destroy_queue(qot);
    return dump_queue(res);
}
