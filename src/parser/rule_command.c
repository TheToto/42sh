#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

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
        return rule_shell_command(tok);
    }
    if ((TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
            || (TOK_TYPE(tok) == WORD && NEXT_TOK_TYPE(tok) == PARENTHESIS_ON))
    {
        return rule_funcdec(tok);
    }
    // HANDLE ERRORS HERE PLEASE
    return rule_simple_command(tok);

    //
    /// -> TODO : REDIRECTIONS FOR FUNCDEC AND SHELL COMMAND
    //

}
