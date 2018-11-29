/**
 *\file rule_redirection.c
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Redirection rule function
 */
#define _GNU_SOURCE
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "shell.h"

static enum redirect_type translate_redirect(enum token_type tok)
{
    switch (tok)
    {
    case LESS:               //<         0
        return R_LESS;
    case GREAT:              //>         1
        return R_GREAT;
    case DLESS:              //<<        2
        return R_DLESS;
    case DGREAT:             //>>        3
        return R_DGREAT;
    case LESSAND:            //<&        4
        return R_LESSAND;
    case GREATAND:           //>&        5
        return R_GREATAND;
    case LESSGREAT:          //<>        6
        return R_LESSGREAT;
    case DLESSDASH:          //<<-       7
        return R_DLESSDASH;
    case CLOBBER:            //>|        8
        return R_CLOBBER;
    default:
        break;
    }
    return R_NONE;
}

static int set_default_io(struct token_list **tok)
{
    switch (TOK_TYPE(tok))
    {
    case LESS:               //<
        return 0;
    case GREAT:              //>
        return 1;
    case DLESS:              //<<
        return 0;
    case DGREAT:             //>>
        return 1;
    case LESSAND:            //<&
        return 0;
    case GREATAND:           //>&
        return 1;
    case LESSGREAT:          //<>
        return 0;
    case DLESSDASH:          //<<-
        return 0;
    case CLOBBER:            //>|
        return 1;
    default:
        return 1;
    }
}

static int skip_token_line(struct token_list **tok)
{
    while (TOK_TYPE(tok) != END_OF_FILE && TOK_TYPE(tok) != NEW_LINE)
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) == END_OF_FILE)
    {
        warnx("End of heredoc missing");
        return 0;
    }
    NEXT_TOK(tok);
    return 1;
}

static int compute_lines_heredoc(struct token_list **tok,
        struct ast_node *redir, char *word)
{
    char *beg = (*tok)->str_origin;

    char *curLine = beg;
    while (curLine)
    {
        char *nextLine = strchr(curLine, '\n');
        if (nextLine)
            *nextLine = '\0';
        if (skip_token_line(tok) == 0)
            return 0;
        if (!strcmp(curLine, word))
            break;
        add_elt_heredoc(redir, curLine);
        if (nextLine)
            *nextLine = '\n';
        curLine = nextLine ? (nextLine+1) : NULL;
    }
    if (curLine == NULL)
    {
        warnx("End of heredoc is missing");
        return 0;
    }
    return 1;
}

static void free_token_list(struct token_list *tok)
{
    if (tok->str)
        free(tok->str);
    free(tok);
}

static void free_save(struct token_list **tok, struct token_list *save_free)
{
    struct token_list *free_until = *tok;
    struct token_list *tok_free = save_free->next;
    save_free->next = free_until;
    while (tok_free != free_until)
    {
        struct token_list *tmp = tok_free->next;
        free_token_list(tok_free);
        tok_free = tmp;
    }
}

static int rule_heredoc(struct token_list **tok, struct ast_node *redir,
        char *word)
{
    if (shell.type == S_PROMPT)
        return 1;
    struct token_list *save = *tok;
    while (TOK_TYPE(tok) != END_OF_FILE && TOK_TYPE(tok) != NEW_LINE)
        NEXT_TOK(tok);
    if (TOK_TYPE(tok) == END_OF_FILE)
    {
        warnx("Heredoc missing");
        return 0;
    }
    struct token_list *save_free = *tok;
    NEXT_TOK(tok); // Skip \n
    if (compute_lines_heredoc(tok, redir, word) == 0)
        return 0;
    free_save(tok, save_free);
    *tok = save;
    return 1;
}

struct ast_node *rule_redirection(struct token_list **tok,
        struct ast_node *child)
{
    int fd = -1;
    int io = -1;
    if (TOK_TYPE(tok) == IO_NUMBER)
    {
        errno = 0;
        io = strtol(TOK_STR(tok), NULL, 10);
        if (errno || io < 0)
        {
            destroy_ast(child);
            warnx("Wrong IO Number");
            return NULL;
        }
        NEXT_TOK(tok);
    }
    else
    {
        io = set_default_io(tok);
    }
    enum redirect_type r_type = translate_redirect(TOK_TYPE(tok));
    if (r_type == R_NONE)
    {
        destroy_ast(child);
        warnx("%s is not a correct redirector", TOK_STR(tok));
        return NULL;
    }
    NEXT_TOK(tok);
    if (TOK_TYPE(tok) != WORD)
    {
        destroy_ast(child);
        warnx("Need a destination in redirection");
        return NULL;
    }
    char *dest = TOK_STR(tok);
    NEXT_TOK(tok);
    struct ast_node *redir = create_ast_node_redirect(fd, r_type, io,
            dest, child);
    if (!rule_heredoc(tok, redir, dest))
    {
        destroy_ast(redir);
        return NULL;
    }
    return redir;
}
