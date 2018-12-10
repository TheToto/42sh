/**
* @file exec_redirect.c
* @author louis.holleville
* @version 1.0
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
#include <sys/wait.h>

#include "env.h"
#include "ast.h"
#include "execution.h"

static void parent_process(int fildes[], struct ast_node_pipe *n,
        struct variables *var, int res)
{
    close(fildes[1]);
    close(STDIN_FILENO);
    dup2(fildes[0], STDIN_FILENO);
    res = exec_node(n->rs, var);
    close(fildes[0]);

    exit(res);
}

static int child_process(int fildes[], struct ast_node_pipe *n,
        struct variables *var, int pid)
{
    int res = 0;
    int pid2 = -1;
    pid2 = fork();
    if (pid2 == -1)
    {
        err(1, "cannot fork in exec_pipe");
    }
    else if (pid2 == 0)
    {
        close(fildes[0]);
        close(STDOUT_FILENO);
        dup2(fildes[1], STDOUT_FILENO);
        res = exec_node(n->ls, var);
        close(fildes[1]);

        exit(res);
    }

    int status2 = 0;
    while (waitpid(pid2, &status2, 0) != pid2)
        continue;
    res = WEXITSTATUS(status2);

    close(fildes[1]);
    close(fildes[0]);

    int status = 0;
    while (waitpid(pid, &status, 0) != pid)
        continue;
    res = WEXITSTATUS(status);
    return res;
}

int exec_pipe(struct ast_node_pipe *n, struct variables *var)
{
    int fildes[2];
    int res = 0;
    if (pipe(fildes) == -1)
        err(1, "cannot pipe in exec_pipe");
    int pid = fork();
    if (pid == -1)
    {
        err(1, "cannot fork in exec_pipe");
    }
    else if (pid == 0)
        parent_process(fildes, n, var, res);
    else
        res = child_process(fildes, n, var, pid);
    return res;
}
