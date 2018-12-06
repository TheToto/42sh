/**
*\file rule_pipe.c
*\author thomas.lupin
*\version 0.8
*\date 22-11-2018
*Pipe rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_pipe(struct token_list **tok)
{
    remove_new_line(tok);
    struct ast_node *left_command = rule_command(tok);
    if (!left_command)
        return NULL;
    if (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        struct ast_node *right_pipe = rule_pipe(tok);
        if (!right_pipe)
        {
            destroy_ast(left_command);
            return NULL;
        }
        return create_ast_node_pipe(left_command, right_pipe);
    }
    return left_command;
}
