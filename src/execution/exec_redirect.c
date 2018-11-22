/**
 * @file exec_redirect.c
 * @author louis.holleville
 * @version 0.5
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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "var.h"
#include "execution.h"
#include "ast.h"

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
    n->fd = open(n->word, O_WRONLY | O_CREAT | O_APPEND, 00644);
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

static int greatand(struct ast_node_redirect *n, struct variables *var)
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

static int dless(struct ast_node_redirect *n, struct variables *var)
{
    n->fd = open(".", O_TMPFILE | O_RDWR, 00644);
    if (n->fd == -1)
        err(1, "cannot open temp doc for heredocs");

    char *line;
    do
    {
        line = readline("> ");
        if (strcmp(line, n->word) == 0)
        {
            free(line);
            break;
        }
        dprintf(n->fd, "%s\n", line);
        free(line);
    }
    while (1);
    lseek(n->fd, 0, SEEK_SET);
    int save = dup(0);
    dup2(n->fd, 0);
    close(n->fd);
    int res = exec_node(n->node, var);
    dup2(save, 0);
    return res;
}

static int dlessdash(struct ast_node_redirect *n, struct variables *var)
{
    n->fd = open(".", O_TMPFILE | O_RDWR, 00644);
    if (n->fd == -1)
        err(1, "cannot open temp doc for heredocs");

    char *line;
    size_t offset;
    do
    {
        offset = 0;
        line = readline("> ");
        for (; line[offset] == '\t' || line[offset] == ' '; offset++);

        if (strcmp(line + offset, n->word) == 0)
        {
            free(line);
            break;
        }
        dprintf(n->fd, "%s\n", line + offset);
        free(line);
    }
    while (1);
    lseek(n->fd, 0, SEEK_SET);
    int save = dup(0);
    dup2(n->fd, 0);
    close(n->fd);
    int res = exec_node(n->node, var);
    dup2(save, 0);
    //unlink("tmp.heredoc");
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
            break;
        case R_GREAT:
            return great(n, var);
            break;
        case R_DLESS:
            return dless(n, var);
            break;
        case R_DGREAT:
            return dgreat(n, var);
            break;
        case R_LESSAND:
            return lessand(n, var);
            break;
        case R_GREATAND:
            return greatand(n, var);
            break;
        case R_LESSGREAT:
            return lessgreat(n, var);
            break;
        case R_DLESSDASH:
            return dlessdash(n, var);
            break;
        case R_CLOBBER:
            return great(n, var);
            break;
        default:
            errx(1, "No fall to switch redirect");
            break;
    }
    return 0;
}
