/**
* \file quote_handling.c
* Handle quotes
* \author Arthur Busuttil
* \version 0.8
* \date 06-12-2018
**/
#include <stdlib.h>
#include <string.h>

#include "quote_lexer.h"
#include "env.h"
#include "shell.h"
#include "queue.h"

struct shell shell;


static void remove_quoting_inside_dquoting(char **str_org);

static int handle_realloc(char **res, char *tmp, size_t *len, int concat)
{
    char *check = realloc(*res, *len + strlen(tmp) + 1);
    if (!check)
    {
        free(*res);
        return 0;
    }
    *len += strlen(tmp);
    *res = check;
    if (concat)
        strcat(*res, tmp);
    return 1;
}

static void handle_single_quote_in_dquote(char *res,
    struct token_list_quote *tl)
{
            strcat(res, "'");
            remove_quoting_inside_dquoting(&tl->str);
            strcat(res, tl->str);
            strcat(res, "'");
}

static void handle_back_slash_in_dquote(char *res,
    struct token_list_quote *tl)
{
    if (*tl->str != '$' && *tl->str != '`' && *tl->str != '\n'
            && *tl->str != '"' && *tl->str != '\\')
        strcat(res, "\\");
    strcat(res, tl->str);
}

static void remove_quoting_inside_dquoting(char **str_org)
{
    char *str = *str_org;
    size_t len = strlen(str);
    char *res = calloc(1, len + 1);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        if (tl->tok == BACK_SLASHED)
            handle_back_slash_in_dquote(res, tl);
        else if (tl->tok > QUOTED)
            strcat(res, tl->str);
        else if (tl->tok == QUOTED)
            handle_single_quote_in_dquote(res, tl);
        else if (tl->tok < DQUOTED)
        {
            char *tmp = get_var(shell.var, tl->str);
            if (tmp)
            {
                if (!handle_realloc(&res, tmp, &len, 1))
                    return;
            }
        }
        tl = tl->next;
    }
    destroy_lexer_quote(l);
    *str_org = res;
    free(str);
}

static void split_space_and_push(struct queue *q, char **res, size_t *len,
    char *tmp)
{
    if (!strlen(*res) && (*tmp == ' ' || *tmp == '\t' || *tmp == '\n'))
    {
        while (*tmp && (*tmp == ' ' || *tmp == '\t' || *tmp == '\n'))
            tmp++;
    }
    while (*tmp)
    {
        if (*tmp == ' ' || *tmp == '\t' || *tmp == '\n')
        {
            push_queue(q, *res);
            free(*res);
            *res = calloc(*len + 1, 1);
        }
        while (*tmp && (*tmp == ' ' || *tmp == '\t' || *tmp == '\n'))
            tmp++;
        while (*tmp && *tmp != ' ' && *tmp != '\t' && *tmp != '\n')
        {
            strncat(*res, tmp, 1);
            tmp++;
        }
    }
}

static int handle_global_dollar_and_dquote(char **res, size_t *len,
    struct token_list_quote *tl, struct queue *q)
{
    char *tmp = "";
    if (tl->tok == DOLLAR)
        tmp = get_var(shell.var, tl->str);
    else
    {
        remove_quoting_inside_dquoting(&tl->str);
        tmp = tl->str;
    }
    if (tmp && !handle_realloc(res, tmp, len, tl->tok != DOLLAR))
        return 0;
    if (tmp && tl->tok == DOLLAR)
    {
        split_space_and_push(q, res, len, tmp);
        if (!**res && !tl->next->next)
        {
            free(*res);
            return 0;
        }
    }
    return 1;
}

void remove_quoting(char *str, struct queue *q)
{
    size_t len = strlen(str);
    int has_not_dollar = 0;
    char *res = calloc(1, len + 1);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        has_not_dollar = has_not_dollar || tl->tok != DOLLAR;
        if (tl->tok >= QUOTED)
            strcat(res, tl->str);
        else if (tl->tok <= DQUOTED
            && !handle_global_dollar_and_dquote(&res, &len, tl, q))
        {
            destroy_lexer_quote(l);
            return;
        }
        tl = tl->next;
    }
    destroy_lexer_quote(l);
    if (!has_not_dollar && *res == 0)
    {
        free(res);
        return;
    }
    push_queue(q, res);
    free(res);
}
