/**
* \file exit.c
* \brief Execute the exit builtin
* \authors sabrina.meng
* \version 0.9
* \date 30-11-2018
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

#include "builtins.h"
#include "env.h"
#include "shell.h"

int exec_exit(char **str)
{
    int val = 0;
    if (!str)
        puts("");
    if (!str || !str[1])
        val = atoi(get_var(shell.var, "?"));
    else
    {
        val = atoi(str[1]) % 255;
        if (!val && !is_zero(str[1]))
        {
            warnx("exit: %s: numeric argument required", str[1]);
            val = 2;
        }
        else if (str[2])
        {
            warnx("exit : too many arguments");
            return 1;
        }
    }
    exit(val);
}
