/**
* \file read.c
* \brief Execute read builtin
* \authors thomas.lupin
* \version 0.9
* \date 05-12-2018
**/
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "builtins.h"
#include "shell.h"
#include "env.h"

int exec_read(char **str)
{
    if (!str[1])
    {
        warnx("Need a var in read");
        return 1;
    }
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1)
    {
        // no lines
        free(line);
        return 1;
    }
    add_var(shell.var, str[1], line, 0);
    free(line);
    return 0;
}
