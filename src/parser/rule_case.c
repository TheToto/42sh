/**
*\file rule_case.c
*\author thomas.lupin
*\version 0.9
*\date 22-11-2018
*\brief Case rule function
*/
#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "queue.h"

static int add_to_node(struct token_list **tok, struct queue *q)
{
    if (!TOK_STR(tok))
    {
        warnx ("Need at least one match item in case item");
        return 0;
    }
    push_queue(q, TOK_STR(tok));
    NEXT_TOK(tok);
    while (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        if (!TOK_STR(tok))
        {
            warnx ("Wrong word item in case item");
            return 0;
        }
        push_queue(q, TOK_STR(tok));
        NEXT_TOK(tok);
    }
    return 1;
}

static int rule_case_item(struct token_list **tok, struct ast_node *case_node)
{
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
    {
        NEXT_TOK(tok);
    }
    struct queue *q = init_queue();
    if (!add_to_node(tok, q))
    {
        destroy_queue(q);
        return 0;
    }
    if (TOK_TYPE(tok) != PARENTHESIS_OFF)
    {
        warnx("Need ')' after a case item");
        destroy_ast(case_node);
        destroy_queue(q);
        return 0;
    }
    NEXT_TOK(tok);
    remove_new_line(tok);
    ask_ps2(tok);
    struct ast_node *exec = NULL;
    if (TOK_TYPE(tok) != DSEMICOLON)
    {
        exec = rule_compound_list(tok, DSEMICOLON);
        if (!exec)
        {
            destroy_ast(case_node);
            destroy_queue(q);
            return 0;
        }
    }
    if (TOK_TYPE(tok) != DSEMICOLON)
    {
        if (exec)
            destroy_ast(exec);
        destroy_ast(case_node);
        destroy_queue(q);
        warnx("Missing ';;' at end of case item");
        return 0;
    }
    NEXT_TOK(tok);
    for (size_t i = 0; i < q->size; i++)
        add_case_value(case_node, q->queue[i], exec);
    destroy_queue(q);
    return 1;;
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
        ask_ps2(tok);
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
    if (TOK_TYPE(tok) != CASE)
    {
        warnx("Need a CASE keyword in a case statement");
        return NULL;
    }
    NEXT_TOK(tok);
    if (!TOK_STR(tok))
    {
        warnx("Need a word to compare in case statement");
        return NULL;
    }
    char *comp = strdup(TOK_STR(tok));
    NEXT_TOK(tok);
    remove_new_line(tok);
    ask_ps2(tok);
    if (TOK_TYPE(tok) != IN)
    {
        warnx("Need a IN keyword in case statement");
        free(comp);
        return NULL;
    }
    NEXT_TOK(tok);
    remove_new_line(tok);
    struct ast_node *case_node = create_ast_node_case(comp);
    free(comp);
    ask_ps2(tok);
    while (TOK_TYPE(tok) != ESAC)
    {
        int i = rule_case_clause(tok, case_node);
        if (i == 0)
            return NULL;
        ask_ps2(tok);
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
