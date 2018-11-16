#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_pipeline(struct token_list **tok)
{
    //printf("Enter in pipeline\n");
    //debug_token(tok);
    short not = 0;
    if (TOK_TYPE(tok) == NOT)
    {
        NEXT_TOK(tok);
        not = 1;
    }
    struct ast_node *left_command = rule_command(tok);
    if (!left_command)
        return NULL;
    struct ast_node *res = left_command;
    if (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        struct ast_node *right_pipe = rule_pipe(tok);
        if (!right_pipe)
            return NULL;
        res = create_ast_node_pipe(left_command, right_pipe);
    }
    if (not)
    {
        return create_ast_node_not(res);
    }
    return res;
}