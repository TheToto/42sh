#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static enum redirect_type translate_redirect(enum token_type tok)
{
    switch(tok)
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
        case PIPE:
            return R_PIPE;
        default:
            break;
    }
    return R_NONE;
}

struct ast_node *rule_redirection(struct token_list **tok,
        struct ast_node *child)
{
    printf("Enter in redirection\n");
    debug_token(tok);
    /// TODO -> RULE REDIRECTION and call this EVERYWHERE.
    int fd = -1;
    if (TOK_TYPE(tok) == IO_NUMBER)
    {
        errno = 0;
        fd = strtol(TOK_STR(tok), NULL, 10);
        if (errno || fd < 0)
        {
            warnx("Wrong IO Number");
            return NULL;
        }
        NEXT_TOK(tok);
    }
    else
    {
        // SET DEFAULT FD
        fd = 1;
    }
    enum redirect_type r_type = translate_redirect(TOK_TYPE(tok));
    if (r_type == R_NONE)
    {
        warnx("%s is not a correct redirector", TOK_STR(tok));
        return NULL;
    }
    NEXT_TOK(tok);
    if (TOK_TYPE(tok) != WORD)
    {
        warnx("Need a destination in redirection");
        return NULL;
    }
    char *dest = TOK_STR(tok);
    NEXT_TOK(tok);
    return create_ast_node_redirect(fd, r_type, fd, dest, child);
}
