/**
*\file rule_for.c
*\author thomas.lupin
*\version 0.5
*\date 22-11-2018
*\brief For rule function
*/
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct ast_node *rule_for(struct token_list **tok)
{
    if (TOK_TYPE(tok) != FOR)
    {
        warnx("Need FOR at begin of for");
        return NULL;
    }
    NEXT_TOK(tok);
    if (TOK_TYPE(tok) != WORD)
    {
        warnx("No var name in FOR");
        return NULL;
    }
    char *value = TOK_STR(tok);
    struct ast_node *for_node = create_ast_node_for(value, NULL);
    if (!for_node)
        return NULL;
    NEXT_TOK(tok);
    remove_new_line(tok);
    ask_ps2(tok);
    if (TOK_TYPE(tok) == IN)
    {
        NEXT_TOK(tok);
        ask_ps2(tok);
        while (TOK_TYPE(tok) == WORD)
        {
            add_value_for(for_node, TOK_STR(tok));
            NEXT_TOK(tok);
        }

        if (TOK_TYPE(tok) != NEW_LINE && TOK_TYPE(tok) != SEMICOLON)
        {
            destroy_ast(for_node);
            warnx("Need ; or \\n after 'in (WORD)*' in for statement");
            return NULL;
        }
        NEXT_TOK(tok);
    }

    remove_new_line(tok);
    ask_ps2(tok);
    struct ast_node *do_group = rule_do_group(tok);
    if (!do_group)
    {
        destroy_ast(for_node);
        return NULL;
    }
    struct ast_node_for *intern = for_node->son;
    intern->exec = do_group;
    return for_node;
}
