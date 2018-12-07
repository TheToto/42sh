/**
* \file quote_lexer.c
* Handle quotes in the lexer
* \author Arthur Busuttil
* \version 0.8
* \date 06-12-2018
**/
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>
#include <err.h>

#include "quote_lexer.h"

static struct lexer_quote *init_lexer(void)
{
    struct lexer_quote *l = NULL;
    l = calloc(1, sizeof(*l));
    if (!l)
        return NULL;
    l->tl = calloc(1, sizeof(*l->tl));
    if (!l->tl)
    {
        free(l);
        return NULL;
    }
    return l;
}

void destroy_lexer_quote(struct lexer_quote *l)
{
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl)
    {
        free(tl->str);
        struct token_list_quote *tmp = tl->next;
        free(tl);
        tl = tmp;
    }
    free(l);
}

static int get_dollar(char **str_org, int *is_quoted, int *is_sub
    , int *is_complete)
{
    *is_complete = 1;
    char *str = *str_org;
    char tmp[2];
    tmp[0] = *str;
    tmp[1] = 0;
    int i = 1;
    if (!fnmatch("[0-9!$?#*@-]", tmp, FNM_EXTMATCH))
        return 1;
    if (*str == '(' || *str == '{')
    {
        *is_sub = *str == '(';
        char first = *str;
        char target = (*str++ == '(') ? ')' : '}';
        *is_quoted = 1;
        (*str_org)++;
        int lvl = 1;
        for (; str[i] && lvl; i++)
        {
            lvl += (*str == first);
            lvl -= (*str == target);
        }
        *is_complete = str[i] != '0';
    }
    else
    {
        i--;
        char *word = calloc(strlen(str) + 1, 1);
        *word = *str;
        while (str[i]
                && !fnmatch("[_a-zA-Z]*([_0-9a-zA-Z])", word, FNM_EXTMATCH))
        {
            i++;
            word[i] = str[i];
        }
        free (word);
    }
    return i;
}

static enum token_quote get_tok_quote(char first)
{
    switch (first)
    {
    case '$':
        return DOLLAR;
    case '\\':
        return BACK_SLASHED;
    case '`':
        return BACK_QUOTED;
    case '\'':
        return QUOTED;
    case '\"':
        return DQUOTED;
    default:
        return WORD_DEFAULT;
    }
    return WORD_DEFAULT;
}

static void update_word(char **str, char *word, int index)
{
    word[index] = **str;
    (*str)++;
}

static char *get_next_word(char **str, enum token_quote *tok, int *is_sub,
    int *is_complete)
{
    char first = **str;
    size_t len = strlen(*str);
    char *word = calloc(1, len + 1);
    *str += (**str && (**str == '\\' || **str == '$'
                || **str == '\"' || **str == '\'' || **str == '`'));
    if (!**str)
        return word;
    if (first == '\\')
        update_word(str, word, 0);
    else if (first == '$')
    {
        int is_quoted = 0;
        int res = get_dollar(str, &is_quoted, is_sub, is_complete);
        strncat(word, *str, res - is_quoted);
        (*str) += res;
    }
    else if (first != '\"' && first != '\'' && first != '`')
    {
        for (int i = 0 ; **str && **str != '\\' && **str != '$'
                && **str != '\"' && **str != '\'' && **str != '`'; i++)
            update_word(str, word, i);
    }
    else
    {
        int is_quoted = 0;
        for (int i = 0; **str && (**str != first
                    || (is_quoted && *(*str) != '\'')); i++)
        {
            is_quoted = !is_quoted && *(*str) == '\\';
            update_word(str, word, i);
        }
        *is_complete = **str != 0;
        (*str) += **str != 0;
    }
    *tok = get_tok_quote(first);
    return word;
}

struct lexer_quote *lexer_quote(char *str)
{
    struct lexer_quote *l = init_lexer();
    if (!l)
        err(1, "Failed to malloc lexer_quote");
    struct token_list_quote *tl = l->tl;
    while (*str)
    {
        char *str_new = get_next_word(&str, &tl->tok, &tl->is_sub
            , &tl->is_complete);
        tl->str = str_new;
        tl->next = calloc(1, sizeof(*tl->next));
        tl = tl->next;
    }
    return l;
}
