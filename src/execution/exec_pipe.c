/**
 * @file exec_redirect.c
 * @author louis.holleville
 * @version 0.5
 * @date 22-11-2018
 * @brief execution of the pipe
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "var.h"
#include "ast.h"
#include "execution.h"

int exec_pipe(struct ast_node_pipe *pipe, struct variables *var)
{
    int fd = open(".", O_TMPFILE | O_RDWR, 00644);
    if (fd == -1)
        err(1, "cannot open temp doc for pipe");

    int save = dup(1);
    dup2(fd, 1);
    int res = exec_node(pipe->ls, var);
    dup2(save, 1);
    lseek(fd, 0, SEEK_SET);
    save = dup(0);
    dup2(fd, 0);
    close(fd);
    res |= exec_node(pipe->rs, var);
    dup2(save, 0);

    return res;
}
