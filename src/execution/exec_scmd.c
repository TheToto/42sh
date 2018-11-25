/**
 * @file exec_scmd.c
 * @author thomas.lupin louis.holleville
 * @version 0.5
 * @date 25-11-2018
 * execution of scmd
 */

#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "execution.h"
#include "parser.h"
#include "ast_destroy.h"

int exec_scmd(struct ast_node_scmd *scmd, struct variables *var)
{
    //if !builtin cmd
    pid_t pid;
    int status;
    int error = 0;
    for (size_t i = 0; i < scmd->pre_size; i++)
        assign_prefix(var, scmd->prefix[i]);
    char **expanded = replace_var_scmd(var, scmd);
    if (scmd->elt_size > 0)
    {
        void *func;
        if ((func = get_func(var, expanded[0])))
            status = exec_node(func, var);
        else
        {
            pid = fork();
            if (pid < 0)
                errx(1, "ERROR: Fork failed");
            else if (pid == 0)
            {
                error = execvp(*expanded, expanded);
                if (error < 0)
                {
                    if (errno == ENOENT)
                        err(127, "Exec %s failed", *expanded);
                    err(126, "Exec %s failed", *expanded);
                }
            }
            else
            {
                while (waitpid(pid, &status, 0) != pid)
                    continue;
            }
            status = WEXITSTATUS(status);
        }
        printf("%s return %d\n", *scmd->elements, status);
    }
    for (size_t i = 0; i < scmd->elt_size + 1; i++)
        free(expanded[i]);
    free(expanded);
    return status;
}
