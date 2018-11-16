#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_simple_command(struct token_list **tok)
{
    //printf("Enter in simple command\n");
    //debug_token(tok);
    struct ast_node *ast_command = create_ast_node_scmd();
    if (!ast_command)
        return NULL;
    rule_prefix(ast_command, tok);
    rule_element(ast_command, tok);
    struct ast_node_scmd *intern_scmd = ast_command->son;
    if (intern_scmd->elt_size + intern_scmd->pre_size < 1)
    {
        destroy_ast(ast_command);
        warnx("Need at least one prefix or one command in simple command");
        return NULL;
    }
    return ast_command;
}

void rule_prefix(struct ast_node *scmd,
        struct token_list **tok)
{
    //printf("Enter in prefix\n");
    //debug_token(tok);
    /// TODO -> RULE  REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == ASSIGNMENT_WORD)
    {
        add_prefix_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
    }
}

void rule_element(struct ast_node *scmd, struct token_list **tok)
{
    //printf("Enter in element\n");
    //debug_token(tok);
    /// TODO -> RULE REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == WORD)
    {
        add_element_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
    }
}
