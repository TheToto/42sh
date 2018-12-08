/**
* \file quote_handling.c
* Handle quotes
* \author Arthur Busuttil
* \version 0.8
* \date 06-12-2018
**/
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "quote_lexer.h"
#include "env.h"
#include "shell.h"
#include "queue.h"

struct shell shell;

static void remove_quoting_inside_dquoting(char **str_org, struct queue *q);

static int handle_realloc(char **res, char *tmp, size_t *len, int concat)
{
    *len += strlen(tmp) * (1 + concat / 2);
    char *check = realloc(*res, *len + 1);
    if (!check)
    {
        free(*res);
        return 0;
    }
    *res = check;
    if (concat == 1)
        strcat(*res, tmp);
    return 1;
}

static void handle_single_quote_in_dquote(char *res,
        struct token_list_quote *tl, struct queue *q)
{
    strcat(res, "\\'");
    remove_quoting_inside_dquoting(&tl->str, q);
    strcat(res, tl->str);
    if (tl->is_complete)
        strcat(res, "\\'");
}

static void handle_back_slash_in_dquote(char *res,
        struct token_list_quote *tl)
{
    strcat(res, "\\");
    if (*tl->str != '$' && *tl->str != '`' && *tl->str != '\n'
            && *tl->str != '"' && *tl->str != '\\')
        strcat(res, "\\");
    strcat(res, tl->str);
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
            if (*tmp == '\\')
                strcat(*res, "\\");
            strncat(*res, tmp, 1);
            tmp++;
        }
    }
}

static void remove_quoting_inside_dquoting(char **str_org, struct queue *q)
{
    char *str = *str_org;
    size_t len = strlen(str) * 2 + 1;
    char *res = calloc(len , 1);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        if (tl->tok == BACK_SLASHED)
            handle_back_slash_in_dquote(res, tl);
        else if (tl->tok > QUOTED)
        {
            for (int i = 0; tl->str[i]; i++)
            {
                strcat(res, "\\");
                strncat(res, tl->str + i, 1);
            }
        }
        else if (tl->tok == QUOTED)
            handle_single_quote_in_dquote(res, tl, q);
        else if (tl->tok < DQUOTED)
        {
            int is_subsh = tl->is_sub && fnmatch("(*(*))", tl->str, FNM_EXTMATCH);
            char *tmp = NULL;
            if (!tl->is_sub)
            {
                tmp = get_var(shell.var, tl->str);
                if (!*tl->str)
                    tmp = "$";
            }
            else
                tmp = get_sub_and_maths(tl->str);
            if (tmp)
            {
                if (!handle_realloc(&res, tmp, &len, 2))
                    return;
                else
                {
                for (int i = 0; tmp[i]; i++)
                    {
                        strcat(res, "\\");
                        strncat(res, tmp + i, 1);
                    }
                }
                if (is_subsh)
                    free(tmp);
            }
        }
        tl = tl->next;
    }
    destroy_lexer_quote(l);
    *str_org = res;
    free(str);
}

static int handle_global_dollar_and_dquote(char **res, size_t *len,
        struct token_list_quote *tl, struct queue *q)
{
    int is_subsh = 0;
    char *tmp = "";
    if (tl->tok == DOLLAR)
    {
        is_subsh = tl->is_sub && fnmatch("(*(*))", tl->str, FNM_EXTMATCH);
        if (!tl->is_sub)
        {
            tmp = get_var(shell.var, tl->str);
            if (!*tl->str)
                tmp = "$";
        }
        else
            tmp = get_sub_and_maths(tl->str);
    }
    else
    {
        remove_quoting_inside_dquoting(&tl->str, q);
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
        if (is_subsh)
            free(tmp);
    }
    return 1;
}

static int path_exp_handling_all(char **res, char *str, size_t *len)
{
    char *check = realloc(*res, *len * 2 + 1);
    if (!check)
    {
        free(*res);
        return 0;
    }
    *len = *len * 2 + 1;
    *res = check;
    while (*str)
    {
        strcat(*res, "\\");
        strncat(*res, str, 1);
        str++;
    }
    return 0;
}

void remove_quoting(char *str, struct queue *q)
{
    size_t len = strlen(str);
    int has_not_dollar = 0;
    char *res = calloc(1, 2 * len + 1);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        has_not_dollar = has_not_dollar || tl->tok != DOLLAR;
        if (tl->tok == WORD_DEFAULT)
            strcat(res, tl->str);
        else if (tl->tok >= QUOTED)
            path_exp_handling_all(&res, tl->str, &len);
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
