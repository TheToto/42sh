#include <stdlib.h>
#include <string.h>

#include "parser.h"

enum token_type get_token_type(char *val)
{
    if (!strcmp(val, "if"))
        return IF;
    if (!strcmp(val, "then"))
        return THEN;
    if (!strcmp(val, "else"))
        return ELSE;
    if (!strcmp(val, "fi"))
        return FI;
    if (!strcmp(val, "while"))
        return WHILE;
    if (!strcmp(val, "for"))
        return FOR;
    if (!strcmp(val, "in"))
        return IN;
    if (!strcmp(val, "do"))
        return DO;
    if (!strcmp(val, "done"))
        return DONE;
    if (!strcmp(val, ";"))
        return SEMICOLON;
    if (!strcmp(val, "&&"))
        return LOGICAL_AND;
    if (!strcmp(val, "||"))
        return LOGICAL_OR;
    return WORD;
}

void lexer_destroy(struct lexer *l)
{
    if (!l)
        return;
    struct token_list *cur = l->token_list;
    while (cur)
    {
        struct token_list *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(l);
}

struct lexer *lexer(char *str)
{
    struct lexer *l = NULL;
    l = calloc(1, sizeof(*l));
    if (!l)
        return NULL;
    l->token_list = malloc(sizeof(*l->token_list));
    if (!l->token_list)
    {
        lexer_destroy(l);
        return NULL;
    }
    struct token_list *cur = l->token_list;
    char *val = strtok(str, " ");
    for (; val; cur = cur->next)
    {
        cur->str = val;
        cur->type = get_token_type(val);
        cur->next = NULL;
        val = strtok(NULL, " ");
        if (val)
        {
            cur->next = calloc(1, sizeof(*cur->next));
            if (!cur->next)
            {
                lexer_destroy(l);
                return NULL;
            }
        }
    }
    return l;
}
