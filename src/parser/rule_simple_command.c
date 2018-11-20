#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static int check_redirect(enum token_type tok)
{
    if (tok <= 8 || tok == IO_NUMBER)
        return 1;
    return 0;
}

static struct ast_node *apply_redir(struct token_list **tok,
        struct ast_node *ast)
{
    struct ast_node *res = ast;
    while (check_redirect(TOK_TYPE(tok)))
    {
        res = rule_redirection(tok, res);
        if (!res)
            return NULL;
    }
    return res;
}

struct ast_node *rule_simple_command(struct token_list **tok)
{
    //printf("Enter in simple command\n");
    debug_token(tok);
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
    //printf("Enter in prefix\n");
    debug_token(tok);
    /// TODO -> RULE  REDIRECTION IF ITS A REDIR

    res = apply_redir(tok, res);
    while (TOK_TYPE(tok) == ASSIGNMENT_WORD)
    {
        add_prefix_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
        res = apply_redir(tok, res);
    }
    return res;
}

struct ast_node *rule_element(struct ast_node *scmd, struct token_list **tok,
        struct ast_node *res)
{
    //printf("Enter in element\n");
    debug_token(tok);
    /// TODO -> RULE REDIRECTION IF ITS A REDIR

    res = apply_redir(tok, res);
    while (TOK_TYPE(tok) == WORD)
    {
        add_element_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
        res = apply_redir(tok, res);
    }
    return res;
}
