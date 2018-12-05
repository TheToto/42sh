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

static int print_hist(void)
{
    HIST_ENTRY **entry = history_list();
    for (size_t i = 1 ; i < history_length; i++)
        printf("%*ld  %s\n", 4, i, entry[i]->line);
    return 0;
}

static int add_hist(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        return 1;
    char *buf = calloc(4096, sizeof(char));
    if (!buf)
    {
        warnx("Calloc failed in add_hist");
        return 1;
    }
    ssize_t size = read(fd, buf, 4096);
    for ( ; size; size = read(fd, buf, 4096))
    {
        if (size == -1)
            return 1;
        buf[size + 1] = '\0';
        add_history(buf);
    }
    close(fd);
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
            return print_hist();
        if (!atoi(str[1]) && strcmp(str[1], "0"))
        {
            warnx("history: %s: numeric value required", str[1]);
            return 1;
        }
    }
    if (opt == 1)
    {
        clear_history();
        return 0;
    }
    if (opt == 2)
        return add_hist(str[1]);
    return 0;
}
