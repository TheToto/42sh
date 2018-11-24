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

static int add_to_node(struct token_list **tok, struct ast_node *case_node,
        struct ast_node *exec)
{
    if (TOK_TYPE(tok) != WORD)
    {
        warnx ("Need at least one match item in case item");
        destroy_ast(exec);
        destroy_ast(case_node);
        return 0;
    }
    add_case_value(case_node, TOK_STR(tok), exec);
    NEXT_TOK(tok);
    while (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        if (TOK_TYPE(tok) != WORD)
        {
            warnx ("Wrong word item in case item");
            destroy_ast(case_node);
            return 0;
        }
        add_case_value(case_node, TOK_STR(tok), exec);
        NEXT_TOK(tok);
    }
    return 1;
}

static int rule_case_item(struct token_list **tok, struct ast_node *case_node)
{
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
        NEXT_TOK(tok);
    struct token_list *save = *tok;
    while (TOK_TYPE(tok) == WORD || TOK_TYPE(tok) == PIPE)
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) != PARENTHESIS_OFF)
    {
        warnx("Need ')' after a case item");
        destroy_ast(case_node);
        return 0;
    }
    NEXT_TOK(tok);
    remove_new_line(tok);
    struct ast_node *exec = NULL;
    if (TOK_TYPE(tok) != DSEMICOLON)
    {
        exec = rule_compound_list(tok);
        if (!exec)
        {
            destroy_ast(case_node);
            return 0;
        }
    }
    if (TOK_TYPE(tok) != DSEMICOLON)
    {
        if (exec)
            destroy_ast(exec);
        destroy_ast(case_node);
        warnx("Missing ';;' at end of case item");
        return 0;
    }
    NEXT_TOK(tok);
    return add_to_node(&save, case_node, exec);
}

static int rule_case_clause(struct token_list **tok,
        struct ast_node *case_node)
{
    int i = rule_case_item(tok, case_node);
    if (i == 0)
        return 0;
    if (TOK_TYPE(tok) == DSEMICOLON)
    {
        NEXT_TOK(tok);
        remove_new_line(tok);
        if (TOK_TYPE(tok) != ESAC)
        {
            i = rule_case_item(tok, case_node);
            if (i == 0)
                return 0;
        }
    }
    remove_new_line(tok);
    return 1;
}

struct ast_node *rule_case(struct token_list **tok)
{
    printf("Enter in case\n");
    debug_token(tok);
    if (TOK_TYPE(tok) != CASE)
    {
        warnx("Need a CASE keyword in a case statement");
        return NULL;
    }
    NEXT_TOK(tok);
    if (TOK_TYPE(tok) != WORD)
    {
        warnx("Need a word to compare in case statement");
        return NULL;
    }
    char *comp = TOK_STR(tok);
    NEXT_TOK(tok);
    remove_new_line(tok);
    if (TOK_TYPE(tok) != IN)
    {
        warnx("Need a IN keyword in case statement");
        return NULL;
    }
    NEXT_TOK(tok);
    remove_new_line(tok);
    struct ast_node *case_node = create_ast_node_case(comp);
    while (TOK_TYPE(tok) != ESAC)
    {
        int i = rule_case_clause(tok, case_node);
        if (i == 0)
            return NULL;
    }
    if (TOK_TYPE(tok) != ESAC)
    {
        destroy_ast(case_node);
        warnx("Need a ESAC keyword in case statement");
        return NULL;
    }
    NEXT_TOK(tok);
    return case_node;
}
