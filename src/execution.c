/**
 *\file execution.c
 *\author sabrina.meng
 *\version 0.3
 *\date 15-11-2018
 *\brief Execution of the AST
 */
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>

#include "execution.h"

/**
 *\fn exec_scmd
 *\brief Execute a simple command
 *\param struct ast_node_scmd *scmd The AST node of the simple command
 *\return Return an int depending on the command
 */
static int exec_scmd(struct ast_node_scmd *scmd)
{
    //if !builtin cmd
    pid_t pid;
    int status;
    int err = 0;

    pid = fork();
    if (pid < 0)
        errx(1, "ERROR: Fork failed");
    else if (pid == 0)
    {
        err = execvp(*scmd->elements, scmd->elements);
        if (err < 0)
            errx(1, "ERROR: Exec failed");
    }
    else
    {
        while (waitpid(pid, &status, 0) != pid)
            continue;
    }
    printf("%s return %d\n", *scmd->elements, status);
    return status;
}

/**
 *\fn exec_if
 *\brief Execute the if command
 *\param struct ast_node_if *n_if   The AST node of the if command
 *\return No return value
 */
static void exec_if(struct ast_node_if *n_if)
{
    int res = exec_node(n_if->condition);
    if (res == 0)
        exec_node(n_if->e_true);
    else
        exec_node(n_if->e_false);
}

/**
 *\fn exec_while
 *\brief Execute the while command
 *\param struct ast_node_while *n_while   The AST node of the while command
 *\return No return value
 */
static void exec_while(struct ast_node_while *n_while)
{
    while (exec_node(n_while->condition) == 0)
        exec_node(n_while->exec);
}

/**
 *\fn exec_for
 *\brief Execute the for command
 *\param struct ast_node_for *n_for   The AST node of the for command
 *\return No return value
 */
static void exec_for(struct ast_node_for *n_for)
{
    n_for = n_for;
}

/**
 *\fn exec_node
 *\brief Execute the complete AST
 *\param struct ast_node *node  The AST node to execute
 *\return Return an int depending on the commands given
 */
int exec_node(struct ast_node *node)
{
    switch (node->type)
    {
        case N_SCMD:
            return exec_scmd(node->son);
        case N_IF:
            exec_if(node->son);
            break;
        case N_WHILE:
            exec_while(node->son);
            break;
        case N_FOR:
            exec_for(node->son);
            break;
        default:
            break;
    }
    return 0;
}
