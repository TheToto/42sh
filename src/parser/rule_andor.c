/**
*\file rule_andor.c
*\author thomas.lupin
*\version 0.8
*\date 22-11-2018
*\brief Andor rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_andor(struct token_list **tok)
{
    struct ast_node *left_pip = rule_pipeline(tok);
    if (!left_pip)
        return NULL;
    if (TOK_TYPE(tok) == LOGICAL_AND || TOK_TYPE(tok) == LOGICAL_OR)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        remove_new_line(tok);
        struct ast_node *right_andor = rule_andor(tok);
        if (!right_andor)
        {
            destroy_ast(left_pip);
            return NULL;
        }
        if (save == LOGICAL_AND)
            return create_ast_node_land(left_pip, right_andor);
        return create_ast_node_lor(left_pip, right_andor);
    }
    return left_pip;
}
