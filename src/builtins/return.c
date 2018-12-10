/**
* \file return.c
* \brief Execute the exit builtin
* \version 1.0
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

#include "builtins.h"
#include "env.h"
#include "shell.h"

int exec_return(char **str)
{
    int val = 0;
    if (!str || !str[1])
        val = atoi(get_var(shell.var, "?"));
    else
    {
        val = atoi(str[1]) % 255;
        if (!val && !is_zero(str[1]))
        {
            warnx("return: %s: numeric argument required", str[1]);
            val = 2;
        }
        else if (str[2])
        {
            warnx("return : too many arguments");
            return 1;
        }
    }
    add_var(shell.var, "?", val, 0);
}
