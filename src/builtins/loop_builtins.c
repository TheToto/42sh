/**
* \file loop_builtins.c
* \brief Execute the continue and break builtins
* \authors sabrina.meng
* \version 1.0
* \date 03-12-2018
**/

#define _GNU_SOURCE

#include <err.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "shell.h"

static int error_check(char **str)
{
    if (!shell.loop)
    {
        warnx("%s: only meaningful in a 'for', 'while', or 'until' loop",
               str[0]);
        return 0;
    }
    if (str[1] && !atoi(str[1]) && !is_zero(str[1]))
    {
        warnx("%s: numeric argument required", str[0]);
        return 128;
    }
    if (get_args(str) > 1)
    {
        warnx("%s: too many arguments", str[0]);
        return 1;
    }
    if (str[1] && atoi(str[1]) < 1)
    {
        warnx("%s: %s: loop count smaller than 1", str[0], str[1]);
        return 1;
    }
    return 0;
}

int exec_continue(char **str)
{
    int n = 1;
    int err = error_check(str);
    if (err)
    {
        shell.n_continue = -1;
        return err;
    }
    n = !str[1] ? 1 : atoi(str[1]);
    shell.n_continue = n;
    return 0;
}

int exec_break(char **str)
{
    int n = 1;
    int err = error_check(str);
    if (err)
    {
        shell.n_break = -1;
        return err;
    }
    n = !str[1] ? 1 : atoi(str[1]);
    shell.n_break = n;
    return 0;
}
