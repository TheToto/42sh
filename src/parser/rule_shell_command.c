/**
*\file rule_shell_command.c
*\author thomas.lupin
*\version 0.8
*\date 22-11-2018
*Shell command rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_shell_command(struct token_list **tok)
{
    if (TOK_TYPE(tok) == IF)
        return rule_if(tok);
    if (TOK_TYPE(tok) == FOR)
        return rule_for(tok);
    if (TOK_TYPE (tok) == WHILE)
        return rule_while(tok);
    if (TOK_TYPE(tok) == UNTIL)
        return rule_until(tok);
    if (TOK_TYPE(tok) == CASE)
        return rule_case(tok);
    if (TOK_TYPE(tok) == BRACKET_ON)
    {
        NEXT_TOK(tok);
        ask_ps2(tok);
        struct ast_node *res = rule_compound_list(tok, BRACKET_OFF);
        if (!res)
            return NULL;
        if (TOK_TYPE(tok) != BRACKET_OFF)
        {
            warnx("Need an '}' after this compound list");
            destroy_ast(res);
            return NULL;
        }
        NEXT_TOK(tok);
        return res;
    }
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
    {
        NEXT_TOK(tok);
        ask_ps2(tok);
        struct ast_node *res = rule_compound_list(tok, PARENTHESIS_OFF);
        if (!res)
            return NULL;
        if (TOK_TYPE(tok) != PARENTHESIS_OFF)
        {
            warnx("Need an ')' after this compound list");
            destroy_ast(res);
            return NULL;
        }
        NEXT_TOK(tok);
        return res;
    }
    warnx("Can't find this shell command");
    return NULL;
}
