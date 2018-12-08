/**
* \file history.c
* Execute history builtin
* \authors sabrina.meng
* \version 0.9
* \date 05-12-2018
**/
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <readline/history.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "builtins.h"
#include "shell.h"
#include "env.h"

static int get_opt(char *opt)
{
    if (!opt || opt[0] != '-')
        return 0;
    if (!strcmp(opt, "-c"))
        return 1;
    if (!strcmp(opt, "-r"))
        return 2;
    return -1;
}

static int print_hist(int offset, char *next)
{
    if (next)
    {
        warnx("history: too many arguments");
        return 1;
    }
    HIST_ENTRY **entry = history_list();
    if (offset >= history_length)
        offset = history_length;
    for (int i = history_length - offset; i < history_length; i++)
        printf("%*d  %s\n", 5, i + 1, entry[i]->line);
    return 0;
}

static int set_histfile(void)
{
    if (get_var(shell.var, "HISTFILE"))
        return 0;
    char *home = get_var(shell.var, "HOME");
    char *file = ".sh_history";
    size_t len = strlen(home) + strlen(file);
    char *path = calloc(len + 2, sizeof(char));
    if (!path)
    {
        warnx("Calloc failed");
        return 1;
    }
    strcat(path, home);
    if (path[strlen(home) - 1] != '/')
        strcat(path, "/");
    strcat(path, file);
    add_var(shell.var, "HISTFILE", path, 0);
    free(path);
    return 0;
}

static int history_r(char *file)
{
    int err = 0;
    if (!file)
    {
        if (set_histfile())
            return 1;
        err = read_history(get_var(shell.var, "HISTFILE"));
    }
    else
        err = read_history(file);
    return err;
}

int exec_history(char **str)
{
    if (shell.type != S_PROMPT)
        using_history();
    int opt = get_opt(str[1]);
    if (opt == -1)
    {
        warnx("history: %s: invalid option\
        \nusage: history [-c] [n] or history -r filename", str[1]);
        return 2;
    }
    if (!opt)
    {
        if (!str[1])
            return print_hist(history_length, NULL);
        if (!atoi(str[1]) && !is_zero(str[1]))
        {
            warnx("history: %s: numeric value required", str[1]);
            return 1;
        }
        return print_hist(atoi(str[1]), str[2]);
    }
    else if (opt == 1)
    {
        clear_history();
        return 0;
    }
    else if (opt == 2)
    {
        if (history_r(str[2]))
            return 1;
    }
    if (shell.type != S_PROMPT)
        using_history();

    return 0;
}
