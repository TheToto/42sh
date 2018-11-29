/**
*\file rulecompound_list.c
*\author thomas.lupin
*\version 0.5
*\date 22-11-2018
*\brief Compound list rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static int check_delim(enum token_type delim, enum token_type t)
{
    if (delim == FI)
        if (t == ELSE || t == ELIF)
            return 1;
    if (delim == t)
        return 1;
    return 0;
}

struct ast_node *rule_compound_list(struct token_list **tok,
        enum token_type delim)
{
    remove_new_line(tok); // skip (\n)*
    struct ast_node *left_andor = rule_andor(tok);
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND
            || TOK_TYPE(tok) == NEW_LINE)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok); // skip & || ;
        remove_new_line(tok);
        // REC
        ask_ps2(tok);
        if (check_delim(delim, TOK_TYPE(tok)))
        {
            if (save == AMPERSAND)
                return create_ast_node_ampersand(left_andor, NULL);
            return left_andor;
        }
        else
        {
            if (save == AMPERSAND)
                return create_ast_node_ampersand(left_andor,
                        rule_compound_list(tok, delim));
            return create_ast_node_semicolon(left_andor,
                    rule_compound_list(tok, delim));
        }
    }
    remove_new_line(tok);
    return left_andor;
}
