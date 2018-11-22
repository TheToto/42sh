/**
 * @file exec_redirect.c
 * @author louis.holleville
 * @version 0.5
 * @date 22-11-2018
 * @brief execution of the pipe
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include "var.h"
#include "ast.h"
#include "execution.h"

int exec_pipe(struct ast_node_pipe *pipe, struct variables *var)
{
    pid_t pid = fork();
    if (pid == -1)
        err(1, "fork failed in exec_pipe");
    if (pid == 0)
        _Exit(exec_node(pipe->rs, var));
    int save = dup(0);
    dup2(1,0);
    int res = exec_node(pipe->ls, var);
    dup2(save, 0);
    int res2;
    wait(&res2);
    return res || res2;
}
