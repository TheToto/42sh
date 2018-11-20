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


static int great(struct ast_node_redirect *n, struct variables *var)
{
    //handle expansion here
    n->fd = open(n->word, O_WRONLY | O_CREAT, 00644);
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

static int dgreat(struct ast_node_redirect *n, struct variables *var)
{
    //handle expansion here
    int res = 0;
    if (n->io_number == 1)
    {
        n->fd = open(n->word, O_WRONLY, 00644);
        if (n->fd == -1)
        {
            warn("cannot redirect with %s", n->word);
            return 1;
        }
        int save1 = dup(1);
        int save2 = dup(2);
        dup2(n->fd, 1);
        dup2(n->fd, 2);
        close(n->fd);
        res = exec_node(n->node, var);
        dup2(save1, n->io_number);
        dup2(save2, n->io_number);
    }
    else
    {
        warnx("ambiguous redirection");
        return 1;
    }
    return res;
}

static int lessgreat(struct ast_node_redirect *n, struct variables *var)
{
    //handle expansion here
    n->fd = open(n->word, O_RDWR | O_CREAT, 00644);
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
            return great(n, var);
            break;
        case R_DLESS:
            break;
        case R_DGREAT:
            return dgreat(n, var);
            break;
        case R_LESSAND:
            break;
        case R_GREATAND:
            break;
        case R_LESSGREAT:
            return lessgreat(n, var);
            break;
        case R_DLESSDASH:
            break;
        case R_CLOBBER:
            return great(n, var);
            break;
        default:
            break;
    }
    return 0;
}

