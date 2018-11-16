#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_funcdec(struct token_list **tok)
{
    //printf("Enter in funcdec\n");
    //debug_token(tok);
    char *name_func;
    if (TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) == WORD)
        name_func = TOK_STR(tok);
    else
    {
        warnx("This function has no name");
        return NULL;
    }
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
        NEXT_TOK(tok);
    else
    {
        warnx("Need a '(' at function declaration");
        return NULL;
    }
    if (TOK_TYPE(tok) == PARENTHESIS_OFF)
        NEXT_TOK(tok);
    else
    {
        warnx("Need a ')' at function declaration");
        return NULL;
    }
    remove_new_line(tok);
    return create_ast_node_fctdec(name_func, rule_shell_command(tok));
}