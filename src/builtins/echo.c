/**
 * \file echo.c
 * Execute the echo builtin
 * \author sabrina.meng
 * \version 0.8
 * \date 02-12-2018
**/

#include <stdio.h>

#include "builtins.h"

int echo(char **str)
{
    char *arg = str[1];
    if (!arg)
        puts("");
    else
    {
        size_t n = get_args(str);
        for (size_t i = 1; i < n; i++)
            printf("%s ", str[i]);
        printf("%s\n", str[n]);
    }
    fflush(stdout);
    return 0;
}
