/**
 *\file rule_while.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief While rule function
 */
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_while(struct token_list **tok)
{
    //printf("Enter in while\n");

    //debug_token(tok);

    if (TOK_TYPE(tok) != WHILE)
    {
        warnx("No while at begining of while...");
        return NULL;
    }
    NEXT_TOK(tok);

    struct ast_node *condition = rule_compound_list(tok);
    if (!condition)
        return NULL;
    if (TOK_TYPE(tok) != DO)
    {
        destroy_ast(condition);
        warnx("Need a do after a while condition");
        return NULL;
    }
    struct ast_node *do_group = rule_do_group(tok);
    if (!do_group)
    {
        destroy_ast(condition);
        return NULL;
    }

    return create_ast_node_while(condition, do_group);
}

struct ast_node *rule_do_group(struct token_list **tok)
{
    //printf("Enter in do group\n");

    //debug_token(tok);

    if (TOK_TYPE(tok) != DO)
    {
        warnx("Need a do in do group");
        return NULL;
    }
    NEXT_TOK(tok);
    struct ast_node *do_group = rule_compound_list(tok);
    if (TOK_TYPE(tok) == DONE)
    {
        NEXT_TOK(tok);
        return do_group;
    }
    destroy_ast(do_group);
    warnx("I need a done after a do group");
    return NULL;
}

struct ast_node *rule_until(struct token_list **tok)
{
    //printf("Enter in until\n");
    //debug_token(tok);
    if (TOK_TYPE(tok) != UNTIL)
    {
        warnx("No until at begining of until...");
        return NULL;
    }
    NEXT_TOK(tok);

    struct ast_node *condition = rule_compound_list(tok);
    if (!condition)
        return NULL;

    if (TOK_TYPE(tok) != DO)
    {
        destroy_ast(condition);
        warnx("Need a do after a while condition");
        return NULL;
    }
    NEXT_TOK(tok);
    struct ast_node *do_group = rule_do_group(tok);
    if (!do_group)
    {
        destroy_ast(condition);
        return NULL;
    }
    struct ast_node *not_cond = create_ast_node_not(condition);
    return create_ast_node_while(not_cond, do_group);
}
