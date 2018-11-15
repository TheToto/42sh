#include <err.h>
#include "parser.h"
#include "ast.h"

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
        return rule_shell_command(tok);
    }
    if ((TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
            || (TOK_TYPE(tok) == WORD && NEXT_TOK_TYPE(tok) == PARENTHESIS_ON))
    {
        return rule_funcdec(tok)
    }
    // HANDLE ERRORS HERE PLEASE
    return rule_simple_command(tok);

    //
    /// -> TODO : REDIRECTIONS FOR FUNCDEC AND SHELL COMMAND
    //

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
    {
        NEXT_TOK(tok);
        struct ast_node *res = rule_compound_list(tok);
        return rule_compound_list(tok);
        if (TOK_TYPE(tok) != BRACKET_OFF)
            // ERROR
            errx(1, "Handle errors better please");
        NEXT_TOK(tok);
        return res;
    }
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
    {
        NEXT_TOK(tok);
        struct ast_node *res = rule_compound_list(tok);
        if (TOK_TYPE(tok) != PARENTHESIS_OFF)
            // ERROR
            errx(1, "Handle errors better please");
        NEXT_TOK(tok);
        return res;
    }
    errx(1, "Handle errors better please");
}


struct ast_node *rule_if(struct token_list **tok)
{
    /// TODO IF
    tok = tok;
    return NULL;
}

struct ast_node *rule_while(struct token_list **tok)
{
    /// TODO WHILE
    tok = tok;
    return NULL;
}

struct ast_node *rule_until(struct token_list **tok)
{
    /// TODO UNTIL
    tok = tok;
    return NULL;
}

struct ast_node *rule_case(struct token_list **tok)
{
    /// TODO CASE
    tok = tok;
    return NULL;
}

struct ast_node *rule_compound_list(struct token_list **tok)
{
    /// TODO COMPOUND LIST
    tok = tok;
    return NULL;
}

struct ast_node *rule_funcdec(struct token_list **tok)
{
    char *name_func;
    if (TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok) == "function"))
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) == WORD)
        name_func = TOK_STR(tok);
    else
        errx(1,"Seriously, handle errors");
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
        NEXT_TOK(tok);
    else
        errx(1,"Seriously, handle errors");
    if (TOK_TYPE(tok) == PARENTHESIS_OFF)
        NEXT_TOK(tok);
    else
        errx(1,"Seriously, handle errors");
    remove_new_line(tok);
    return parse_shell_command(tok);
}

struct ast_node *rule_simple_command(struct token_list **tok)
{
    struct ast_node *ast_command = create_ast_node_scmd(NULL, NULL);
    rule_prefix(ast_command, tok);
    rule_element(ast_command, tok);
    /// TODO -> VERIF SIZE prefix + element MUST BE > 0
    return ast_command;
}

struct ast_node *rule_compound_prefix(struct ast_node *scmd,
        struct token_list **tok)
{
    /// TODO -> RULE  REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == ASSIGNMENT_WORD)
    {
        scmd_add_prefix(TOK_STR(tok));
        NEXT_TOK(tok);
    }
}

struct ast_node *rule_compound_element(struct token_list **tok)
{
    /// TODO -> RULE REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == WORD)
    {
        scmd_add_element(TOK_STR(tok));
        NEXT_TOK(tok);
    }
}

struct ast_node *rule_compound_redirection(struct token_list **tok)
{
    /// TODO -> RULE REDIRECTION and call this EVERYWHERE.
    tok = tok;
    return NULL;
}
