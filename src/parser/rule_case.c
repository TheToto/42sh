/**
 *\file rule_case.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Case rule function
 */
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_case(struct token_list **tok)
{
    printf("Enter in case\n");
    debug_token(tok);
    /// TODO CASE
    warnx("TODO : rule_case");
    tok = tok;
    return NULL;
}
