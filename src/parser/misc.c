/**
*\file misc.c
*\author thomas.lupin
*\version 1.0
*\date 28-11-2018
*\brief Some misc functions
*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "options.h"
#include "shell.h"

void debug_token(struct token_list **tok)
{
    printf("[DEBUG TOKEN] ");
    struct token_list *t = *tok;
    while (t)
    {
        if (t->str)
            printf("%s : ", t->str);
        else
            printf("EOF");
        t = t->next;
    }
    printf("\n");
}

void remove_new_line(struct token_list **tok)
{
    while (TOK_TYPE(tok) == NEW_LINE)
    {
        NEXT_TOK(tok);
    }
}

void ask_ps2(struct token_list **tok)
{
    if (shell.type != S_PROMPT)
        return;
    while (TOK_TYPE(tok) == END_OF_FILE)
    {
        struct token_list *lex_tok = show_ps2();
        *tok = lex_tok;
    }
}
