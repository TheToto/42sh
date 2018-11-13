#pragma once

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

enum option
{
    NONE = 0,
    CMD,
    NORC,
    AST,
    VERSION,
};

void check_options(char *argv[]);
