/**
 * \file exit.c
 * Execute the exit builtin
 * \authors sabrina.meng
 * \version 0.8
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
    size_t n = get_args(str);
    char *arg = str[1];
    int val;
    printf("exit\n");
    if (!n)
        val = atoi(get_var(shell.var, "?"));
    else
    {
        val = atoi(arg) % 255;
        if (!val && !is_zero(arg))
            val = 2;
        else if (n != 1)
        {
            warnx("exit : too many arguments");
            return 0;
        }
    }
    exit(val);
}
