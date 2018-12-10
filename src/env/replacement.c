/**
* \file replacement.c
* \author louis.holleville
* \version 1.0
* \date 10-12-2018
* \brief Management of replacement functions
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

char *get_sub_and_maths(char *name)
{
    if (*name && !fnmatch("(*(*))", name, FNM_EXTMATCH))
        return get_maths(name);
    else
        return redirect_subshell(name);
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

static void special_var(char *in, struct queue *q, size_t i)
{
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
}

static void check_string_at(char *in, struct queue *q)
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
                    special_var(in, q, i);
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

static char **check_at(char **input)
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
    for (size_t i = 0; n_for->values[i]; i++, j++)
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

