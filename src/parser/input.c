#include <err.h>
#include <stdio.h>

#include "parser.h"
#include "ast.h"

static void debug_token(struct token_list **tok)
{
    struct token_list *t = *tok;
    while (t)
    {
        printf("%d", t->type);
        if (t->str)
            printf(": %s", t->str);
        printf("\n");
        t = t->next;
    }
}

void remove_new_line(struct token_list **tok)
{
printf("Enter in newline\n");
debug_token(tok);
    while (TOK_TYPE(tok) == NEWLINE)
        NEXT_TOK(tok);
}

struct ast_node *rule_input(struct token_list **tok)
{
printf("Enter in input\n");
debug_token(tok);
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        // DO NOTHING (Not an error)
        return NULL;
    }
    //NEXT_TOK(tok);
    struct ast_node *res = rule_list(tok);
    if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
    {
        return res;
    }
    // ERROR
    return NULL;
}

struct ast_node *rule_list(struct token_list **tok)
{
printf("Enter in list\n");
debug_token(tok);
    struct ast_node *left_andor = rule_andor(tok);
    if (!left_andor)
        // ERROR
        return NULL;
    if (TOK_TYPE(tok) == SEMICOLON || TOK_TYPE(tok) == AMPERSAND)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        if (TOK_TYPE(tok) == NEWLINE || TOK_TYPE(tok) == END_OF_FILE)
        {
            if(save == AMPERSAND)
                return create_ast_node_ampersand(left_andor, NULL);
            return left_andor;
        }
        struct ast_node *right_list = rule_list(tok);
        if (save == SEMICOLON)
            return create_ast_node_semicolon(left_andor, right_list);
        return create_ast_node_ampersand(left_andor, right_list);
    }
    return left_andor;
}

struct ast_node *rule_andor(struct token_list **tok)
{
printf("Enter in andor\n");
debug_token(tok);
    struct ast_node *left_pip = rule_pipeline(tok);
    if (!left_pip)
        // ERROR
        return NULL;
    if (TOK_TYPE(tok) == LOGICAL_AND || TOK_TYPE(tok) == LOGICAL_OR)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok);
        remove_new_line(tok);
        struct ast_node *right_andor = rule_andor(tok);
        if (save == LOGICAL_AND)
            return create_ast_node_land(left_pip, right_andor);
        return create_ast_node_lor(left_pip, right_andor);
    }
    return left_pip;
}

struct ast_node *rule_pipeline(struct token_list **tok)
{
printf("Enter in pipeline\n");
debug_token(tok);
    short not = 0;
    if (TOK_TYPE(tok) == NOT)
    {
        NEXT_TOK(tok);
        not = 1;
    }
    struct ast_node *left_command = rule_command(tok);
    struct ast_node *res = left_command;
    if (TOK_TYPE(tok) == PIPE)
    {
        NEXT_TOK(tok);
        struct ast_node *right_pipe = rule_pipe(tok);
        res = create_ast_node_pipe(left_command, right_pipe);
    }
    if (not)
    {
        return create_ast_node_not(res);
    }
    return res;
}

struct ast_node *rule_pipe(struct token_list **tok)
{
printf("Enter in pipe\n");
debug_token(tok);
    remove_new_line(tok);
    struct ast_node *left_command = rule_command(tok);
    if (TOK_TYPE(tok) == PIPE)
    {
        struct ast_node *right_pipe = rule_pipe(tok);
        return create_ast_node_pipe(left_command, right_pipe);
    }
    return left_command;
}

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

struct ast_node *rule_shell_command(struct token_list **tok)
{
printf("Enter in shell command\n");
debug_token(tok);
    if (TOK_TYPE(tok) == IF)
        return rule_if(tok);
    if (TOK_TYPE(tok) == FOR)
        return rule_for(tok);
    if (TOK_TYPE (tok) == WHILE)
        return rule_while(tok);
    if (TOK_TYPE(tok) == UNTIL)
        return rule_until(tok);
    if (TOK_TYPE(tok) == CASE)
        return rule_case(tok);
    if (TOK_TYPE(tok) == BRACKET_ON)
    {
        NEXT_TOK(tok);
        struct ast_node *res = rule_compound_list(tok);
        return rule_compound_list(tok);
        if (TOK_TYPE(tok) != BRACKET_OFF)
            // ERROR
            errx(1, "Handle errors better please");
        NEXT_TOK(tok);
        return res;
    }
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
    {
        NEXT_TOK(tok);
        struct ast_node *res = rule_compound_list(tok);
        if (TOK_TYPE(tok) != PARENTHESIS_OFF)
            // ERROR
            errx(1, "Handle errors better please");
        NEXT_TOK(tok);
        return res;
    }
    errx(1, "Handle errors better please");
}


struct ast_node *rule_if(struct token_list **tok)
{
printf("Enter in if\n");
debug_token(tok);
    NEXT_TOK(tok);
    struct ast_node *condition = rule_compound_list(tok);
    if (TOK_TYPE(tok) != THEN)
        errx(1, "Error no then after if : %d", TOK_TYPE(tok));
    NEXT_TOK(tok);
    struct ast_node *e_true = rule_compound_list(tok);

    struct ast_node *e_false = NULL;
    if (TOK_TYPE(tok) == ELIF || TOK_TYPE(tok) == ELSE)
        e_false = rule_else_clause(tok);

    if (TOK_TYPE(tok) != FI)
        errx(2, "Error no fi at end of if statement");
    NEXT_TOK(tok);
    return create_ast_node_if(e_true, e_false, condition);
}

struct ast_node *rule_else_clause(struct token_list **tok)
{
    if (TOK_TYPE(tok) == ELSE)
    {
        NEXT_TOK(tok); // skip ELSE
        return rule_compound_list(tok);
    }
    if (TOK_TYPE(tok) != ELIF)
        errx(1,"Error, no else no elfi");

    NEXT_TOK(tok); // skip ELFI
    struct ast_node *condition = rule_compound_list(tok);
    if (TOK_TYPE(tok) != THEN)
        errx(1, "Error no then after if");
    NEXT_TOK(tok); // skip THEN
    struct ast_node *e_true = rule_compound_list(tok);

    struct ast_node *e_false = NULL;
    if (TOK_TYPE(tok) == ELIF || TOK_TYPE(tok) == ELSE)
        e_false = rule_else_clause(tok);

    return create_ast_node_if(e_true, e_false, condition);
}

struct ast_node *rule_for(struct token_list **tok)
{
printf("Enter in for\n");
debug_token(tok);
    /// TODO IF
    tok = tok;
    return NULL;
}

struct ast_node *rule_while(struct token_list **tok)
{
printf("Enter in while\n");
debug_token(tok);
    if (TOK_TYPE(tok) != WHILE)
        errx(1, "Wtf dude ?");
    NEXT_TOK(tok);

    struct ast_node *condition = rule_compound_list(tok);
    if (TOK_TYPE(tok) != DO)
        errx(1, "I need a do after a while cond");
    NEXT_TOK(tok);
    struct ast_node *do_group = rule_do_group(tok);

    return create_ast_node_while(condition, do_group);
}

struct ast_node *rule_do_group(struct token_list **tok)
{
    struct ast_node *do_group = rule_compound_list(tok);
    if (TOK_TYPE(tok) == DONE)
    {
        NEXT_TOK(tok);
        return do_group;
    }
    errx(1, "I need a done after a do group");
}

struct ast_node *rule_until(struct token_list **tok)
{
printf("Enter in until\n");
debug_token(tok);
    if (TOK_TYPE(tok) != UNTIL)
        errx(1, "Wtf dude ?");
    NEXT_TOK(tok);

    struct ast_node *condition = rule_compound_list(tok);
    if (TOK_TYPE(tok) != DO)
        errx(1, "I need a do after a while cond");
    NEXT_TOK(tok);
    struct ast_node *do_group = rule_do_group(tok);

    struct ast_node *not_cond = create_ast_node_not(condition);
    return create_ast_node_while(not_cond, do_group);
}

struct ast_node *rule_case(struct token_list **tok)
{
printf("Enter in case\n");
debug_token(tok);
    /// TODO CASE
    tok = tok;
    return NULL;
}

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
            || TOK_TYPE(tok) == NEWLINE)
    {
        enum token_type save = TOK_TYPE(tok);
        NEXT_TOK(tok); // skip & || ;
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
    return left_andor;
}

struct ast_node *rule_funcdec(struct token_list **tok)
{
printf("Enter in funcdec\n");
debug_token(tok);
    char *name_func;
    if (TOK_TYPE(tok) == WORD && !strcmp(TOK_STR(tok), "function"))
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) == WORD)
        name_func = TOK_STR(tok);
    else
        errx(1,"Seriously, handle errors");
    if (TOK_TYPE(tok) == PARENTHESIS_ON)
        NEXT_TOK(tok);
    else
        errx(1,"Seriously, handle errors");
    if (TOK_TYPE(tok) == PARENTHESIS_OFF)
        NEXT_TOK(tok);
    else
        errx(1,"Seriously, handle errors");
    remove_new_line(tok);
    return create_ast_node_fctdec(name_func, rule_shell_command(tok));
}

struct ast_node *rule_simple_command(struct token_list **tok)
{
printf("Enter in simple command\n");
debug_token(tok);
    struct ast_node *ast_command = create_ast_node_scmd();
    rule_prefix(ast_command, tok);
    rule_element(ast_command, tok);
    /// TODO -> VERIF SIZE prefix + element MUST BE > 0
    return ast_command;
}

void rule_prefix(struct ast_node *scmd,
        struct token_list **tok)
{
printf("Enter in prefix\n");
debug_token(tok);
    /// TODO -> RULE  REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == ASSIGNMENT_WORD)
    {
        add_prefix_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
    }
}

void rule_element(struct ast_node *scmd, struct token_list **tok)
{
printf("Enter in element\n");
debug_token(tok);
    /// TODO -> RULE REDIRECTION IF ITS A REDIR

    while (TOK_TYPE(tok) == WORD)
    {
        add_element_scmd(scmd, TOK_STR(tok));
        NEXT_TOK(tok);
    }
}

struct ast_node *rule_compound_redirection(struct token_list **tok)
{
printf("Enter in redirection\n");
debug_token(tok);
    /// TODO -> RULE REDIRECTION and call this EVERYWHERE.
    tok = tok;
    return NULL;
}
