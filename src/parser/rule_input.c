#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

void debug_token(struct token_list **tok)
{
    struct token_list *t = *tok;
    while (t)
    {
        printf("%d", t->type);
        if (t->str)
            printf(": %s", t->str);
        printf("\n");
        t = t->next;
    }
}

void remove_new_line(struct token_list **tok)
{
    //printf("Enter in newline\n");
    //debug_token(tok);
    while (TOK_TYPE(tok) == NEWLINE)
        NEXT_TOK(tok);
}

struct ast_node *rule_input(struct token_list **tok)
{
    //printf("Enter in input\n");
    //debug_token(tok);
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        // DO NOTHING (Not an error)
        return NULL;
    }
    struct ast_node *res = rule_list(tok);
    if (!res)
        return NULL;
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        return res;
    }
    warnx("Your input is malformed.");
    return NULL;
}
