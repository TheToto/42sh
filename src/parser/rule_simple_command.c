/**
*\file rule_simple_command.c
*\author thomas.lupin
*\version 0.9
*\date 22-11-2018
*\brief Simple command rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static int check_redirect(struct token_list **tok)
{
    if (TOK_TYPE(tok) <= 8
            || (TOK_TYPE(tok) == IO_NUMBER && NEXT_TOK_TYPE(tok) <= 8))
        return 1;
    return 0;
}

static struct ast_node *apply_redir(struct token_list **tok,
        struct ast_node *ast)
{
    struct ast_node *res = ast;
    while (check_redirect(tok))
    {
        res = rule_redirection(tok, res);
        if (!res)
            return NULL;
    }
    return res;
}

static int check_delim(enum token_type tok)
{
    if (tok < 10 || tok == SEMICOLON || tok == DSEMICOLON
            || tok == PARENTHESIS_ON || tok == PARENTHESIS_OFF
            || tok == LOGICAL_AND || tok == LOGICAL_OR
            || tok == AMPERSAND || tok == END_OF_FILE
            || tok == NEW_LINE)
        return 1;
    return 0;
}

struct ast_node *rule_simple_command(struct token_list **tok)
{
    struct ast_node *ast_command = create_ast_node_scmd();
    struct ast_node *res = ast_command;
    if (!ast_command)
        return NULL;
    res = rule_prefix(ast_command, tok, res);
    if (!res)
        return NULL;
    res = rule_element(ast_command, tok, res);
    if (!res)
        return NULL;
    struct ast_node_scmd *intern_scmd = ast_command->son;
    if (intern_scmd->elt_size + intern_scmd->pre_size < 1)
    {
        destroy_ast(ast_command);
        warnx("Need at least one prefix or one command in simple command");
        return NULL;
    }
    return res;
}

struct ast_node *rule_prefix(struct ast_node *scmd, struct token_list **tok,
        struct ast_node *res)
{
    res = apply_redir(tok, res);
    if (!res)
        return NULL;
    while (TOK_TYPE(tok) == ASSIGNMENT_WORD)
    {
        add_prefix_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
        res = apply_redir(tok, res);
        if (!res)
            return NULL;
    }
    return res;
}

struct ast_node *rule_element(struct ast_node *scmd, struct token_list **tok,
        struct ast_node *res)
{
    res = apply_redir(tok, res);
    if (!res)
        return NULL;
    while (!check_delim(TOK_TYPE(tok)))
    {
        add_element_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
        res = apply_redir(tok, res);
        if (!res)
            return NULL;
    }
    return res;
}
