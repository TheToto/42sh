/**
 *\file rule_input.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Input rule function
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

void debug_token(struct token_list **tok)
{
    return;
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

    while (TOK_TYPE(tok) == NEW_LINE)
        NEXT_TOK(tok);
}

struct ast_node *rule_input(struct token_list **tok)
{
    //printf("Enter in input\n");
    //debug_token(tok);

    remove_new_line(tok);
    if (TOK_TYPE(tok) == NEW_LINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        // DO NOTHING (Not an error)
        struct ast_node *empty = malloc(sizeof(struct ast_node));
        empty->type = N_NONE;
        empty->son = NULL;
        return empty;
    }
    struct ast_node *res = rule_list(tok);
    if (!res)
        return NULL;
    remove_new_line(tok);
    if (TOK_TYPE(tok) == END_OF_FILE)
    {
        return res;
    }
    else
    {
        struct ast_node *right_input = rule_input(tok);
        if (!right_input)
        {
            destroy_ast(res);
            return NULL;
        }
        return create_ast_node_semicolon(res, right_input);
    }
    warnx("Your input is malformed.");
    return NULL;
}
