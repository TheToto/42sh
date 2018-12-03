/**
*\file prompt.c
*\author thomas.lupin sabrina.meng
*\version 0.5
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

#include "execution.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "readfile.h"
#include "shell.h"
#include "builtins.h"
#include "shopt.h"

static char *init_path(char *file)
{
    size_t len = strlen(getenv("HOME")) + strlen(file) + 1;
    char *histpath = calloc(len, sizeof(char));
    strcat(histpath, getenv("HOME"));
    strcat(histpath, file);
    return histpath;
}

static void write_hist(char *file)
{
    struct stat buf;
    if (!stat(file, &buf))
        append_history(history_length, file);
    else
        write_history(file);
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

struct token_list *show_ps2(void)
{
    char *buf = readline(get_var(shell.var, "PS2"));
    lexer_destroy(shell.lexer);
    struct lexer *l = lexer(buf);
    free(buf);
    return l->token_list;
}

static void init_libvar(void)
{
    if (!get_var(shell.var, "PS1"))
        add_var(shell.var, "PS1", "[42sh@pc]$ ", 1);
    if (!get_var(shell.var, "PS2"))
        add_var(shell.var, "PS2", "> ", 1);
    add_var(shell.var, "PWD", getenv("PWD"), 1);
    add_var(shell.var, "HOME", getenv("HOME"), 1);
}

char *quote_ps2(void)
{
    char *buf = readline(get_var(shell.var, "PS2"));
    size_t size_buf = strlen(shell.buf);
    char *tmp = realloc(shell.buf,
            (size_buf + strlen(buf) + 2) * sizeof(char));
    if (!tmp)
        err(1, "Failled to realloc quote ps2");
    shell.buf = tmp;
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
    init_libvar();
    while (1)
    {
        char *buf = readline(get_var(shell.var, "PS1"));
        if (buf && *buf)
        {
            add_history(buf);
            write_hist(histpath);
            history_truncate_file(histpath, 500);
        }
        shell.buf = buf;
        exec_main(buf, is_print, shell.var);
        shell.buf = NULL;
        free(buf);
    }
    free(histpath);
    return 0;
}
