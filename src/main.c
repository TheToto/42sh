/**
*\file main.c
*\author thomas.lupin
*\version 0.5
*\date 22-11-2018
*\brief Main function
*/
#include <stdio.h>
#include <err.h>
#include <shell.h>
#include <stdlib.h>
#include <time.h>

#include "options.h"
#include "lexer.h"
#include "env.h"
#include "ast_destroy.h"

struct shell shell =
{
    S_UNDEFINED,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

void my_abort(void)
{
    if (shell.ast)
        destroy_ast(shell.ast);
    if (shell.var)
        destroy_var(shell.var);
    if (shell.lexer)
        lexer_destroy(shell.lexer);
    if (shell.buf)
        free(shell.buf);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (atexit(my_abort) != 0)
        errx(1, "Failed to set up atexit");
    if (argc < 1)
    {
        warnx("Invalid usage");
        errx(1, "Usage: ./42sh [options] [file]");
    }
    options(argv);
    return 0;
}
