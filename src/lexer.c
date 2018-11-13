#include <stdlib.h>
#include <string.h>

#include "parser.h"

enum token_type get_token_type(char *val)
{
    if (!strcmp(val, "if"))
        return FI;
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

void my_abort(struct lexer *l)
{
    if (!l)
        return;
    struct token_list *cur = l->token_list;
    while (cur)
    {
        free(cur->str);
        struct token_list *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(l);
}

void print_token_list(struct token_list *tl)
{
#include <stdio.h>
    while (tl)
    {
        printf("%d\n", tl->type);
        tl = tl->next;
    }
}

struct lexer *lexer(char *cmd)
{
    struct lexer *l = NULL;
    l = calloc(1, sizeof(*l));
    if (!l)
        return NULL;
    l->token_list = malloc(sizeof(*l->token_list));
    if (!l->token_list)
    {
        my_abort(l);
        return NULL;
    }
    struct token_list *cur = l->token_list;
    char *val = strtok(cmd, " ");
    while (val)
    {
        cur->str = val;
        cur->type = get_token_type(val);
        cur->next = NULL;
        val = strtok(cmd, " ");
        if (val)
        {
            cur->next = malloc(sizeof(*cur->next));
            if (!cur->next)
            {
                my_abort(l);
                return NULL;
            }
        }
    }
    print_token_list(l->token_list);
    return l;
}
