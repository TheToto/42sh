/**
*\file prompt.c
*\author thomas.lupin sabrina.meng
*\version 1.0
*\date 22-11-2018
*\brief Function to use the prompt
*/

#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <err.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#include "execution.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "readfile.h"
#include "shell.h"
#include "builtins.h"
#include "shopt.h"

static char *redirect_readline(char *var)
{
    int save = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);
    char *buf = readline(advanced_prompt(var));
    dup2(save, STDOUT_FILENO);
    close(save);
    return buf;
}

static char *init_path(char *file)
{
    size_t len = strlen(getenv("HOME")) + strlen(file) + 1;
    char *histpath = calloc(len, sizeof(char));
    strcat(histpath, getenv("HOME"));
    strcat(histpath, file);
    return histpath;
}

static void write_hist(void)
{
    char *histpath = init_path("/.42sh_history");
    struct stat buf;
    if (!stat(histpath, &buf))
        append_history(history_length, histpath);
    else
        write_history(histpath);
    history_truncate_file(histpath, 500);
    free(histpath);
}

static void launchrc(int is_print, struct variables *var)
{
    char *filerc = init_path("/.42shrc");
    struct stat buf;
    if (!stat("/etc/42shrc", &buf))
        launch_file("/etc/42shrc", is_print, var);
    if (!stat(filerc, &buf))
        launch_file(filerc, is_print, var);

    free(filerc);
}

static void catch_ctrlc(int signo)
{
    if (signo == SIGINT)
    {
        add_var(shell.var, "?", "130", 0);
        int save = dup(STDOUT_FILENO);
        dup2(STDERR_FILENO, STDOUT_FILENO);
        puts("");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        dup2(save, STDOUT_FILENO);
        close(save);
    }
}

struct token_list *show_ps2(void)
{
    char *buf = redirect_readline("PS2");
    lexer_destroy(shell.lexer);
    struct lexer *l = lexer(buf);
    free(buf);
    return l->token_list;
}

char *quote_ps2(void)
{
    int save = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);
    char *buf = NULL;
    while (!buf)
        buf = readline(advanced_prompt("PS2"));
    dup2(save, STDOUT_FILENO);
    close(save);
    size_t size_buf = strlen(shell.buf);
    char *tmp = calloc(size_buf + strlen(buf) + 2, sizeof(char));
    strcat(tmp, shell.buf);
    free(shell.buf);
    if (!tmp)
        err(1, "Failed to realloc quote ps2");
    shell.buf = tmp;
    strcat(shell.buf, "\n");
    strcat(shell.buf, buf);
    free(buf);
    return shell.buf + size_buf;
}

int show_prompt(int norc, int is_print)
{
    char *histpath = init_path("/.42sh_history");
    read_history(histpath);
    if (!norc)
        launchrc(is_print, shell.var);
    atexit(write_hist);

    struct sigaction sa;
    sa.sa_handler = catch_ctrlc;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    while (1)
    {
        char *buf = redirect_readline("PS1");
        if (!buf)
            exec_exit(NULL);
        if (*buf)
            add_history(buf);
        shell.buf = buf;
        exec_main(buf, is_print, shell.var);
        free(shell.buf);
        shell.buf = NULL;
    }
    free(histpath);
    return 0;
}
