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
    if (!str[1])
        return 1;
    add_var(shell.var, "?", str[1], 0);
    return atoi(str[1]);
}
