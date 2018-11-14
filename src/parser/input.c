#include <err.h>
#include "parser.h"
#include "ast.h"

#define NEXT_TOK(Tok) (*Tok = (*Tok)->next)

#define TOK_TYPE(Tok) ((*Tok)->type)
#define TOK_STR(Tok) ((*Tok)->str)
#define NEXT_TOK_TYPE(Tok) ((*Tok)->next->type)
#define NEXT_TOK_STR(Tok) ((*Tok)->next->str)

void remove_new_line(struct token_list **tok)
{
    while (TOK_TYPE(tok) == NEWLINE)
        NEXT_TOK(tok);
}

struct ast_node *rule_input(struct token_list **tok)
{
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        // DO NOTHING (Not an error)
        return NULL;
    }
    NEXT_TOK(tok);
    struct ast_node *res = rule_list(tok);
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        return res;
    }
    // ERROR
    return NULL;
}

struct ast_node *rule_list(struct token_list **tok)
{
    struct ast_node *left_andor = rule_andor(tok);
    if (!left_andor)
        // ERROR
        return NULL;
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
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
    if (TOK_TYPE(tok) == BRACKET_ON
            || TOK_TYPE(tok) == PARENTHESIS_ON
            || TOK_TYPE(tok) == FOR
            || TOK_TYPE(tok) == WHILE
            || TOK_TYPE(tok) == UNTIL
            || TOK_TYPE(tok) == CASE
            || TOK_TYPE(tok) == IF)
    {
        //NEXT_TOK(tok);
        return rule_shell_command(tok);
    }
    if ((TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
            || (TOK_TYPE(tok) == WORD && NEXT_TOK_TYPE(tok) == PARENTHESIS_ON))
    {
        return rule_funcdec(tok)
    }
    // HANDLE ERRORS HERE PLEASE
    return rule_simple_command(tok);
}

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
        return rule_compound_list(tok);
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
    {
        NEXT_TOK(tok);
        struct ast_node *res = rule_compound_list(tok);
        if (TOK_TYPE(tok) != BRACKET_OFF && TOK_TYPE(tok) != PARENTHESIS_OFF)
            // ERROR
            errx(1, "Handle errors better please");
        NEXT_TOK(tok);
        return res;
    }
    errx(1, "Handle errors better please");
}


struct ast_node *rule_if(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_while(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_until(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_case(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_compound_list(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_funcdec(struct token_list **tok)
{
    errx(1,"Func dec missing");
    return NULL;
}

struct ast_node *rule_simple_command(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_compound_prefix(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_compound_element(struct token_list **tok)
{
    return NULL;
}

struct ast_node *rule_compound_redirection(struct token_list **tok)
{
    return NULL;
}

