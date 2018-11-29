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
#include <stdlib.h>

#include "env.h"
#include "ast.h"
#include "execution.h"

int exec_pipe(struct ast_node_pipe *n, struct variables *var)
{
    int fildes[2];
    int res = 0;
    int save;
    if (pipe(fildes) == -1)
        err(1, "cannot pipe in exec_pipe");
    switch (fork())
    {
    case -1:
        err(1, "cannot fork in exec_pipe");
        break;
    case 0:
        close(fildes[1]);
        save = dup(0);
        dup2(fildes[0], 0);
        res = exec_node(n->rs, var);
        dup2(save, 0);
        exit(res);
        break;
    default:
        close(fildes[0]);
        save = dup(1);
        dup2(fildes[1], 1);
        res = exec_node(n->ls, var);
        dup2(save, 1);
        break;
    }
    return res;
}
