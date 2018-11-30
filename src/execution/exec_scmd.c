/**
* @file exec_scmd.c
* @author thomas.lupin louis.holleville sabrina.meng
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
#include "quote_lexer.h"

static void add_params(char **expanded, struct variables *var)
{
    for (int i = 1; expanded[i] != NULL; i++)
    {
        char *buf = calloc(50, sizeof(char));
        if (!buf)
            err(1, "Malloc failed in add_params");
        sprintf(buf, "%d", i);
        add_var(var, buf, expanded[i]);
        free(buf);
    }
}

static void urgent_free(char **expanded)
{
    char exit_str[256] =
    {
        0
    };
    strncpy(exit_str, *expanded, 255);
    for (size_t i = 0; expanded[i]; i++)
        free(expanded[i]);
    free(expanded);
    if (errno == ENOENT)
        err(127, "Exec %s failed", exit_str);
    err(126, "Exec %s failed", exit_str);

}

static int execute(char **expanded, int status, struct variables *var)
{
    pid_t pid;
    int error = 0;
    void *func = NULL;
    if ((func = get_func(var, expanded[0])))
    {
        add_params(expanded, var);
        status = exec_node(func, var);
    }
    else
    {
        pid = fork();
        if (pid < 0)
            errx(1, "ERROR: Fork failed");
        else if (pid == 0)
        {
            error = execvp(*expanded, expanded);
            if (error < 0)
                urgent_free(expanded);
        }
        else
        {
            while (waitpid(pid, &status, 0) != pid)
                continue;
        }
        status = WEXITSTATUS(status);
    }
    return status;
}

int exec_scmd(struct ast_node_scmd *scmd, struct variables *var)
{
    //if !builtin cmd
    int status = 0;
    for (size_t i = 0; i < scmd->pre_size; i++)
        assign_prefix(var, scmd->prefix[i]);
    for (size_t i = 0; i < scmd->elt_size; i++)
        remove_quoting(&(scmd->elements[i]));
    if (scmd->elt_size > 0)
    {
        status = execute(scmd->elements, status, var);
    }
    return status;
}
