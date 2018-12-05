/**
 * \file source.c
 * Execute the source | . builtins
 * \authors sabrina.meng
 * \version 0.8
 * \date 03-12-2018
**/

#include <err.h>

#include "builtins.h"
#include "shell.h"
#include "readfile.h"
#include "env.h"

int source(char **str)
{
    char *filename = str[1];
    if (!filename)
    {
        warnx("%s: filename argument required\
        \n%s: usage: %s filename [arguments]", str[0], str[0], str[0]);
        return 2;
    }
    set_up_var(str + 1);
    launch_file(filename, 0, shell.var);
    return 0;
}
