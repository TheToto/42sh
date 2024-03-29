/**
* @file exec_redirect.c
* @author louis.holleville
* @version 1.0
* @date 20-11-2018
* @brief execution of the redirections
*/

#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "env.h"
#include "execution.h"
#include "ast.h"
#include "shell.h"
#include "queue.h"

struct shell shell;

static int less(struct ast_node_redirect *n, struct variables *var)
{
    char *exp = concat_quote(n->word);
    n->fd = open(exp, O_RDONLY);
    free(exp);
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
    close(save);
    return res;
}


static int great(struct ast_node_redirect *n, struct variables *var)
{
    char *expand = concat_quote(n->word);
    n->fd = open(expand, O_WRONLY | O_CREAT, 00644);
    free(expand);
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
    close(save);
    return res;
}

static int dgreat(struct ast_node_redirect *n, struct variables *var)
{
    char *exp = concat_quote(n->word);
    n->fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 00644);
    free(exp);
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
    close(save);
    return res;
}

static int greatand(struct ast_node_redirect *n, struct variables *var)
{
    int res = 0;
    if (n->io_number == 1)
    {
        char *exp = concat_quote(n->word);
        n->fd = open(exp, O_WRONLY | O_CREAT, 00644);
        free(exp);
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
        dup2(save1, 1);
        dup2(save2, 2);
        close(save1);
        close(save2);
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
    char *exp = concat_quote(n->word);
    n->fd = open(exp, O_RDWR | O_CREAT, 00644);
    free(exp);
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
    close(save);
    return res;
}

static int dless(struct ast_node_redirect *n, struct variables *var)
{
    n->fd = open("/tmp", O_TMPFILE | O_RDWR, 00644);
    if (n->fd == -1)
        err(1, "cannot open temp doc for heredocs");

    for (size_t i = 0; i < n->size; i++)
        dprintf(n->fd, "%s\n", n->words[i]);

    lseek(n->fd, 0, SEEK_SET);
    int save = dup(0);
    dup2(n->fd, 0);
    close(n->fd);
    int res = exec_node(n->node, var);
    dup2(save, 0);
    close(save);
    return res;
}

static int dlessdash(struct ast_node_redirect *n, struct variables *var)
{
    n->fd = open("/tmp", O_TMPFILE | O_RDWR, 00644);
    if (n->fd == -1)
        err(1, "cannot open temp doc for heredocs");

    for (size_t i = 0; i < n->size; i++)
        dprintf(n->fd, "%s\n", n->words[i]);

    lseek(n->fd, 0, SEEK_SET);
    int save = dup(0);
    dup2(n->fd, 0);
    close(n->fd);
    int res = exec_node(n->node, var);
    dup2(save, 0);
    close(save);
    return res;
}

static int lessand(struct ast_node_redirect *n, struct variables *var)
{
    n = n;
    var = var;
    warnx("ambiguous redirection");
    return 1;
}

int exec_redirect(struct ast_node_redirect *n, struct variables *var)
{
    switch (n->type)
    {
    case R_LESS:
        return less(n, var);
    case R_GREAT:
        return great(n, var);
    case R_DLESS:
        return dless(n, var);
    case R_DGREAT:
        return dgreat(n, var);
    case R_LESSAND:
        return lessand(n, var);
    case R_GREATAND:
        return greatand(n, var);
    case R_LESSGREAT:
        return lessgreat(n, var);
    case R_DLESSDASH:
        return dlessdash(n, var);
    case R_CLOBBER:
        return great(n, var);
    default:
        errx(1, "No fall to switch redirect");
    }
    return 0;
}
