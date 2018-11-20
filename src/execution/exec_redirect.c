/**
 * @file exec_redirect.c
 * @author louis.holleville
 * @version 0.5
 * @date 20-11-2018
 * @brief execution of the redirections
 */

#include <err.h>
#include <errno.h>
#include <unistd.h>
#include "ast.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parser.h"
#include "var.h"
#include "execution.h"

//also handle GREAT
static int less(struct ast_node_redirect *n, struct variables *var)
{
    //handle expansion here
    n->fd = open(n->word, O_RDONLY);
    if (n->fd == -1)
    {
        warn("cannot redirect with %s", n->word);
        return 1;
    }
    int save = dup(n->io_number);
    dup2(n->fd, n->io_number);
    close(n->fd);
    int res = exec_node(n->node, var);
    dup2(save, n->io_number);
    return res;
}

int exec_redirect(struct ast_node_redirect *n, struct variables *var)
{
    switch (n->type)
    {
        case R_LESS:
            return less(n, var);
            break;
        case R_GREAT:
            return less(n, var);
            break;
        case R_DLESS:
            break;
        case R_DGREAT:
            break;
        case R_LESSAND:
            break;
        case R_GREATAND:
            break;
        case R_LESSGREAT:
            break;
        case R_DLESSDASH:
            break;
        case R_CLOBBER:
            break;
        case R_PIPE:
            break;
        default:
            break;
    }
    return 0;
}

