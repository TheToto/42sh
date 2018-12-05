/**
* @file exec_scmd.c
* @author thomas.lupin louis.holleville sabrina.meng
* @version 0.5
* @date 25-11-2018
* execution of scmd
*/

#define _GNU_SOURCE
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
#include "builtins.h"
#include "quote_lexer.h"

static char *itoa(int i, char *buf_nb)
{

    sprintf(buf_nb, "%d", i);
    return buf_nb;
}

static void add_params(char **expanded, struct variables *var)
{
    char buf_nb[20] =
    {
        0
    };
    int nb = 1;
    int size = 0;
    for (; expanded && expanded[nb] && nb < 100; nb++)
    {
        add_var(var, itoa(nb, buf_nb), expanded[nb], 0);
        size += strlen(expanded[nb]) + 1;
    }
    add_var(var, "#", itoa(nb - 1, buf_nb), 0);
    for(; nb < 100; nb++)
        add_var(var, itoa(nb, buf_nb), "", 0);
    // $@ $*
    char *star = calloc(size + 1, sizeof(char));
    for (size_t i = 1; expanded && expanded[i]; i++)
    {
        strcat(star, expanded[i]);
        if (expanded[i + 1])
            strcat(star, " ");
    }
    add_var(var, "*", star, 0);
    add_var(var, "@", star, 0);
    free(star);
}

static int exec_func(char **expanded, struct variables *var, void *func)
{
    int status = 0;
    char buf_nb[20] =
    {
        0
    };
    char **backup = calloc(100, sizeof(char*));

    // BACKUP VARS
    for (int i = 0; i < 99; i++)
    {
        char *b = get_var(var, itoa(i, buf_nb));
        if (b)
            backup[i] = strdup(b);
    }
    char *sharp = strdup(get_var(var, "#"));
    char *star = strdup(get_var(var, "*"));
    char *arob = strdup(get_var(var, "@"));

    add_params(expanded, var);
    status = exec_node(func, var);

    // RESTORE VARS
    for (int i = 0; i < 99; i++)
    {
        if (backup[i])
        {
            add_var(var, itoa(i, buf_nb), backup[i], 0);
            free(backup[i]);
        }
        else
            add_var(var, itoa(i, buf_nb), "", 0);
    }
    free(backup);
    add_var(var, "#", sharp, 0);
    add_var(var, "*", star, 0);
    add_var(var, "@", arob, 0);
    free(sharp);
    free(star);
    free(arob);
    return status;
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
        status = exec_func(expanded, var, func);
    else if ((status = exec_builtin(expanded)) != -1)
        status = status;
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
    char ret[10];
    sprintf(ret, "%d", status);
    add_var(var, "?", ret, 0);
    return status;
}

int exec_scmd(struct ast_node_scmd *scmd, struct variables *var)
{
    int status = 0;
    for (size_t i = 0; i < scmd->pre_size; i++)
        assign_prefix(var, scmd->prefix[i]);
    char **expanded = replace_var_scmd(scmd);
    if (scmd->elt_size > 0)
    {
        status = execute(expanded, status, var);
    }
    for (size_t i = 0; expanded[i]; i++)
        free(expanded[i]);
    free(expanded);
    return status;
}
