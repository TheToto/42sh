#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "options.h"

// Get the option type accordig to the enum
static enum option get_option(char *opt, size_t section)
{
    if (!section)
    {
        if (!strcmp(opt, "--norc"))
            return NORC;
        if (!strcmp(opt, "--ast-print"))
            return AST;
        if (!strcmp(opt, "--version"))
            return VERSION;
        if (!strcmp(opt, "-c"))
            return CMD;
        if (!strcmp(opt, "-O") || !strcmp(opt, "+O"))
            return SHOPT;
    }
    return NONE;
}

// Check if all options are correct
static int check_options(char *argv[])
{
    enum option opt = NONE;
    size_t section = 0;

    for (size_t i = 1; argv[i]; i++)
    {
        section = argv[i][0] == '-' ? 0 : 1;
        opt = get_option(argv[i], section);
        switch (opt)
        {
            case CMD:
                if (!(argv[++i] && argv[i][0] != '-'))
                {
                    warnx("Invalid arguments for -c option");
                    errx(1, "Usage: -c <command>");
                }
                break;
            case SHOPT:
                break;
            case NORC:
                break;
            case AST:
                break;
            case VERSION:
                break;
            default:
                if (section != 1)
                {
                    warnx("Unrecognized option");
                    errx(1, "Usage: ./42sh [options] [file]");
                }
        }
    }
    return 1;
}

// Do actions according to each options
void options(char *argv[])
{
    if (!check_options(argv))
        return;

    enum option opt = NONE;
    size_t section = 0;

    for (size_t i = 1; argv[i]; i++)
    {
        section = argv[i][0] == '-' ? 0 : 1;
        opt = get_option(argv[i], section);
        switch (opt)
        {
            case CMD:
                printf("-c args OK\n");
                break;
            case SHOPT:
                //activates shopt variables
                break;
            case NORC:
                //deactivates ressource reader
                break;
            case AST:
                //ast print
                break;
            case VERSION:
                printf("Version 0.3\n");
                exit(0);
            default:
                printf("All good\n");
        }
    }
}
