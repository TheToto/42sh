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

int show_prompt(int norc, int is_print)
{
    char *histpath = init_path("/.42sh_history");
    struct variables *library = init_var();
    putenv("PS1=[42sh@pc]$ ");
    if (!norc)
        launchrc(is_print, library);
    while (1)
    {
        char *buf = readline(getenv("PS1"));
        if (buf && *buf)
            add_history(buf);
        if (!strcmp(buf, "exit"))
        {
            free(buf);
            break;
        }
        exec_main(buf, is_print, library);
        free(buf);
    }
    write_hist(histpath);
    history_truncate_file(histpath, 500);

    destroy_var(library);
    free(histpath);
    return 0;
}
