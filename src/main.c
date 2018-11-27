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

#include "options.h"

struct shell shell =
{
    S_UNDEFINED,
    NULL,
    NULL
};

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        warnx("Invalid usage");
        errx(1, "Usage: ./42sh [options] [file]");
    }
    options(argv);
}
