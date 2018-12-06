/**
* \file history.c
* Execute history builtin
* \authors sabrina.meng
* \version 0.8
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

int exec_history(char **str)
{
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
        int err = read_history(str[2]);
        if (err)
            return 1;
    }
    return 0;
}
