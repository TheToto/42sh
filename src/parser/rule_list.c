#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_list(struct token_list **tok)
{
    //printf("Enter in list\n");
    //debug_token(tok);
    struct ast_node *left_andor = rule_andor(tok);
    if (!left_andor)
        return NULL;
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
        {
            if(save == AMPERSAND)
                return create_ast_node_ampersand(left_andor, NULL);
            return left_andor;
        }
        struct ast_node *right_list = rule_list(tok);
        if (!right_list)
            return NULL;
        if (save == SEMICOLON)
            return create_ast_node_semicolon(left_andor, right_list);
        return create_ast_node_ampersand(left_andor, right_list);
    }
    return left_andor;
}