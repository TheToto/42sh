/**
 *\file rule_command.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Command rule function
 */
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

struct ast_node *rule_command(struct token_list **tok)
{
    printf("Enter in command\n");
    debug_token(tok);
    if (TOK_TYPE(tok) == BRACKET_ON
            || TOK_TYPE(tok) == PARENTHESIS_ON
            || TOK_TYPE(tok) == FOR
            || TOK_TYPE(tok) == WHILE
            || TOK_TYPE(tok) == UNTIL
            || TOK_TYPE(tok) == CASE
            || TOK_TYPE(tok) == IF)
    {
        struct ast_node *res = rule_shell_command(tok);
        while (check_redirect(TOK_TYPE(tok)))
        {
            res = rule_redirection(tok, res);
            if (!res)
                return NULL;
        }
        return res;
    }
    if ((TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
            || (TOK_TYPE(tok) == WORD && NEXT_TOK_TYPE(tok) == PARENTHESIS_ON))
    {
        struct ast_node *res = rule_funcdec(tok);
        while (check_redirect(TOK_TYPE(tok)))
        {
            res = rule_redirection(tok, res);
            if (!res)
                return NULL;
        }
        return res;
    }
    // HANDLE ERRORS HERE PLEASE
    return rule_simple_command(tok);

    //
    /// -> TODO : REDIRECTIONS FOR FUNCDEC AND SHELL COMMAND
    //

}
