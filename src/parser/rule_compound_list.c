/**
 *\file rulecompound_list.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Compound list rule function
 */
#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static int check_andor(enum token_type t)
{
    return (t == DSEMICOLON
            || t == PARENTHESIS_OFF
            || t == SEMICOLON
            || t == END_OF_FILE
            || t == BRACKET_OFF
            || t == DO
            || t == FI
            || t == DONE
            || t == ELSE
            || t == ELIF
            || t == ESAC
            || t == THEN) ? 1 : 0;
}

struct ast_node *rule_compound_list(struct token_list **tok)
{
    printf("Enter in compound list\n");
    debug_token(tok);
    remove_new_line(tok); // skip (\n)*
    struct ast_node *left_andor = rule_andor(tok);
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND
            || TOK_TYPE(tok) == NEW_LINE)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok); // skip & || ;
        remove_new_line(tok);
        // REC
        if (check_andor(TOK_TYPE(tok)))
        {
            if (save == AMPERSAND)
                return create_ast_node_ampersand(left_andor, NULL);
            return left_andor;
        }
        else
        {
            if (save == AMPERSAND)
                return create_ast_node_ampersand(left_andor,
                        rule_compound_list(tok));
            return create_ast_node_semicolon(left_andor,
                    rule_compound_list(tok));;
        }
    }
    remove_new_line(tok);
    return left_andor;
}
