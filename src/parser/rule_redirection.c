#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"


struct ast_node *rule_compound_redirection(struct token_list **tok)
{
    printf("Enter in redirection\n");
    debug_token(tok);
    /// TODO -> RULE REDIRECTION and call this EVERYWHERE.
    warnx("TODO: rule_redirection");
    tok = tok;
    return NULL;
}
