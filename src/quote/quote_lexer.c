#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "quote_lexer.h"

static
struct lexer_quote *init_lexer(void)
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

int is_dollar(char *str)
{
    if (!fnmatch("[0-9!$-?#*@]", str, FNM_EXTMATCH))
        return 1;
    if (!fnmatch("[_a-zA-Z]*([_0-9a-zA-Z])", str, FNM_EXTMATCH))
        return 2;
    return 0;
}

static
enum token_quote get_tok_quote(char first)
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

static
char *get_next_word(char **str, enum token_quote *tok)
{
    char first = **str;
    size_t len = strlen(*str);
    char *word = calloc(1, len);
    *str += (**str && (**str == '\\' || **str == '$'
                || **str == '\"' || **str == '\'' || **str == '`'));
    if (first == '\\')
    {
        *word = **str;
        (*str)++;
    }
    else if (first == '$')
    {
        int i = 0;
        int res = 2;
        for (; **str && res == 2; i++)
        {
            word[i] = **str;
            res = is_dollar(word);
            if (res == 0)
                break;
            (*str)++;
        }
        word[i + res % 2] = 0;
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
        return NULL;
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
