#pragma once

enum option
{
    NONE = 0,
    SHOPT,
    CMD,
    NORC,
    AST,
    VERSION,
};

void options(char *argv[]);
