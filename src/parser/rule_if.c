/**
*\file rule_if.c
*\author thomas.lupin
*\version 1.0
*\date 22-11-2018
*\brief If rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static struct ast_node *rule_if_bis(struct token_list **tok,
        struct ast_node *e_true, struct ast_node *condition)
{
    struct ast_node *e_false = NULL;
    ask_ps2(tok);
    if (TOK_TYPE(tok) == ELIF || TOK_TYPE(tok) == ELSE)
    {
        e_false = rule_else_clause(tok);
        if (!e_false)
        {
            destroy_ast(condition);
            destroy_ast(e_true);
            return NULL;
        }
    }
    if (TOK_TYPE(tok) != FI)
    {
        destroy_ast(e_true);
        destroy_ast(e_false);
        destroy_ast(condition);
        warnx("Error no fi at end of if statement");
        return NULL;
    }
    NEXT_TOK(tok);
    return create_ast_node_if(e_true, e_false, condition);
}

struct ast_node *rule_if(struct token_list **tok)
{
    NEXT_TOK(tok);
    ask_ps2(tok);
    struct ast_node *condition = rule_compound_list(tok, THEN);
    if (!condition)
        return NULL;
    ask_ps2(tok);
    if (TOK_TYPE(tok) != THEN)
    {
        destroy_ast(condition);
        warnx("No then after if statement");
        return NULL;
    }
    NEXT_TOK(tok);
    ask_ps2(tok);
    struct ast_node *e_true = rule_compound_list(tok, FI);
    if (!e_true)

    {
        destroy_ast(condition);
        return NULL;
    }
    return rule_if_bis(tok, e_true, condition);
}

static struct ast_node *rule_else_clause_bis(struct token_list **tok,
        struct ast_node *condition)
{
    ask_ps2(tok);
    struct ast_node *e_true = rule_compound_list(tok, FI);
    if (!e_true)
    {
        destroy_ast(condition);
        return NULL;
    }
    struct ast_node *e_false = NULL;
    ask_ps2(tok);
    if (TOK_TYPE(tok) == ELIF || TOK_TYPE(tok) == ELSE)
    {
        e_false = rule_else_clause(tok);
        if (!e_false)
        {
            destroy_ast(e_true);
            destroy_ast(e_false);
            destroy_ast(condition);
            return NULL;
        }
    }
    return create_ast_node_if(e_true, e_false, condition);
}

struct ast_node *rule_else_clause(struct token_list **tok)
{
    if (TOK_TYPE(tok) == ELSE)
    {
        NEXT_TOK(tok); // skip ELSE
        ask_ps2(tok);
        return rule_compound_list(tok, FI);
    }
    if (TOK_TYPE(tok) != ELIF)
    {
        warnx("No else, no elif at else clause");
        return NULL;
    }

    NEXT_TOK(tok); // skip ELIF
    ask_ps2(tok);
    struct ast_node *condition = rule_compound_list(tok, THEN);
    if (!condition)
        return NULL;
    ask_ps2(tok);
    if (TOK_TYPE(tok) != THEN)
    {
        destroy_ast(condition);
        warnx("No then after elif");
        return NULL;
    }
    NEXT_TOK(tok); // skip THEN
    return rule_else_clause_bis(tok, condition);
}
