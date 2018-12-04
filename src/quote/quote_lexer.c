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

int get_dollar(char **str_org, int *is_quoted)
{
    char *str = *str_org;
    char tmp[2];
    tmp[0] = *str;
    tmp[1] = 0;
    int i = 1;
    if (!fnmatch("[0-9!$?#*@-]", tmp, FNM_EXTMATCH))
        return 1;
    if (*str == '(' || *str == '{')
    {
        char target = (*str == '(') ? ')' : '}';
        *is_quoted = 1;
        (*str_org)++;
        for (; str[i]; i++)
        {
            if (str[i] == target)
                break;
        }
    }
    else
    {
        i--;
        char *word = calloc(strlen(str) + 1, 1);
        *word = *str;
        while (str[i] && !fnmatch("[_a-zA-Z]*([_0-9a-zA-Z])", word, FNM_EXTMATCH))
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

static char *get_next_word(char **str, enum token_quote *tok)
{
    char first = **str;
    size_t len = strlen(*str);
    char *word = calloc(1, len + 1);
    *str += (**str && (**str == '\\' || **str == '$'
                || **str == '\"' || **str == '\'' || **str == '`'));
    if (first == '\\')
    {
        *word = **str;
        (*str)++;
    }
    else if (first == '$')
    {
        int is_quoted = 0;
        int res = get_dollar(str, &is_quoted);
        strncat(word, *str, res - is_quoted);
        (*str) += res;
    }
    else if (first != '\"' && first != '\'' && first != '`')
    {
        for (int i = 0 ; **str && **str != '\\' && **str != '$'
            && **str != '\"' && **str != '\'' && **str != '`'; i++)
        {
            word[i] = **str;
            (*str)++;
        }
    }
    else
    {
        for (int i = 0; **str && (**str != first || *(*str - 1) == '\\'); i++)
        {
            word[i] = **str;
            (*str)++;
        }
        (*str)++;
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
        char *str_new = get_next_word(&str, &tl->tok);
        tl->str = str_new;
        tl->next = calloc(1, sizeof(*tl->next));
        tl = tl->next;
    }
    return l;
}
