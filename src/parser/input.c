#include "parser.h"
#include "ast.h"

#define NEXT_TOK(Tok) (*Tok = (*Tok)->next)

#define TOK_TYPE(Tok) (*(Tok)->type)
#define NEXT_TOK_TYPE(Tok) (*(Tok)->next->type)

void remove_new_line(struct token_list **tok)
{
    while (TOK_TYPE(tok) == NEWLINE)
        NEXT_TOK(tok);
}

struct ast_node *rule_input(struct token_list **tok)
{
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == EOF)
    {
        // DO NOTHING (Not an error)
        return NULL;
    }
    NEXT_TOK(tok);
    struct ast_node *res = rule_list(tok);
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == EOF)
    {
        return res;
    }
    // ERROR
    return NULL;
}

struct ast_node *rule_list(struct token_list **tok)
{
    struct ast_node *left_andor = rule_andor(tok);
    if (!res_andor)
        // ERROR
        return NULL;
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == EOF)
        {
            if(save == AMPERSAND)
                return create_ast_node_ampersand(left_andor, NULL);
            return lhs;
        }
        struct ast_node *right_list = parse_list(tok);
        if (save == SEMICOLON)
            return create_ast_node_semicolon(left_andor, right_list);
        return create_ast_node_ampersand(left_andor, right_list);
    }
    return left_andor;
}

struct ast_node *rule_andor(struct token_list **tok)
{
    struct ast_node *left_pip = rule_pipeline(tok);
    if (!res_pip)
        // ERROR
        return NULL;
    if (TOK_TYPE(tok) == LOGICAL_AND || TOK_TYPE(tok) == LOGICAL_OR)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        remove_new_line(tok);
        struct ast_node *right_andor = rule_andor(tok);
        if (save == LOGICAL_AND)
            return create_ast_node_land(left_pip, right_andor);
        return create_ast_node_lor(left_pip, right_andor);
    }
    return res_pip;
}

struct ast_node *rule_pipeline(struct token_list **tok)
{
    short not = 0;
    if (TOK_TYPE(tok) == NOT)
    {
        NEXT_TOK(tok);
        not = 1;
    }
    struct ast_node *left_command = rule_command(tok);
    struct ast_node *res = left_command;
    if (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        struct ast_node *right_pipe = rule_pipe(tok);
        res = create_ast_node_pipe(left_command, right_pipe);
    }
    if (not)
    {
        return create_ast_node_not(res);
    }
    return res;
}

struct ast_node *rule_pipe(struct token_list **tok)
{
    remove_new_line(tok);
    struct ast_node *left_command = rule_command(tok);
    if (TOK_TYPE(tok) == PIPE)
    {
        struct ast_node *right_pipe = rule_pipe(tok);
        return create_ast_node_pipe(left_command, right_pipe);
    }
    return left_command;
}

struct ast_node *rule_command(struct token_list **tok)
{
    return NULL;
}
