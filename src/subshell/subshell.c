/**
* @file subshell.c
* @author thomas.lupin louis.holleville
* @date 07-12-2018
* @version 0.9
* Management of subshells
*/

#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "ast.h"
#include "subshell.h"
#include "execution.h"
#include "ast_destroy.h"
#include "env.h"
#include "shell.h"
#include "lexer.h"

static void lauch_subshell(char *input)
{
    char *back_pid = strdup(get_var(shell.var, "$"));
    int pid = fork();
    if (pid < 0)
    {
        err(1, "fatal : fork failed");
    }
    else if (pid == 0)
    {
        if (shell.ast)
            destroy_ast(shell.ast);
        if (shell.var)
            destroy_var(shell.var);
        if (shell.lexer)
            lexer_destroy(shell.lexer);
        if (shell.buf && shell.type != S_OPTION)
            free(shell.buf);
        shell.buf = input;
        shell.type = S_FILE;
        struct variables *lib = init_var();
        add_var(lib, "$", back_pid, 0);
        free(back_pid);
        int res = exec_main(input, 0, lib);
        destroy_var(lib);
        exit(res);
    }
    free(back_pid);
    int status = 0;
    while (waitpid(pid, &status, 0) != pid)
        continue;
    status = WEXITSTATUS(status);
    char ret[10];
    sprintf(ret, "%d", status);
    add_var(shell.var, "?", ret, 0);
}

char *redirect_subshell(char *input)
{
    *input = 0;//avoiding warning TO BE REMOVE
    *output = 0;//avoiding warning TO BE REMOVE
    int fd = open("/tmp", O_TMPFILE | O_RDWR);
    int save = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    lauch_subshell(input);

    dup2(save, STDOUT_FILENO);
    char *output = fd_to_string(fd); //is malloced
    return output;
}
