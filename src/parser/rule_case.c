#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_case(struct token_list **tok)
{
    //printf("Enter in case\n");
    debug_token(tok);
    /// TODO CASE
    warnx("TODO : rule_case");
    tok = tok;
    return NULL;
}
