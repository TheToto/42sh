/**
 *\file options.c
 *\author sabrina.meng
 *\version 0.3
 *\date 15-11-2018
 *\brief Options parsing
 *\detail Parse the options with the format
 *[GNU long option] [option] script-file
 */

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "options.h"

/**
 *\fn get_section
 *\brief Check if it is long option, option or script-file
 *\param char *arg  The argument to check
 *\return Return a size_t representing the section
 */
static size_t get_section(char *arg)
{
    if (arg[0] == '-')
    {
        if (arg[1] && arg[1] == '-')
            return 0;
        return 1;
    }
    return 2;
}

/**
 *\fn get_option
 *\brief Get the option type according to the enum of the header
 *\param char *opt  The option to check
 *\return Return an enum value acoording to the option
 */
static enum option get_option(char *opt)
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
    return NONE;
}

/**
 *\fn options
 *\brief Do actions according to each options
 *\param char *argv[]   The command line to parse
 *\return The return value depends on the options
 */
void options(char *argv[])
{
    enum option opt = NONE;
    size_t section = 0;

    for (size_t i = 1; argv[i]; i++)
    {
        size_t sect = get_section(argv[i]);
        section = section > sect ? section : sect;
        opt = get_option(argv[i]);
        switch (opt)
        {
            case CMD:
                if (section == 1 && !(argv[++i] && argv[i][0] != '-'))
                {
                    warnx("Invalid arguments for -c option");
                    errx(1, "Usage: -c <command>");
                }
                break;
            case SHOPT:
                //if (!section)
                //activates shopt variables
                break;
            case NORC:
                //if (!section)
                //deactivates ressource reader
                break;
            case AST:
                //if (!section)
                //ast print
                break;
            case VERSION:
                if (!section)
                {
                    printf("Version 0.3\n");
                    exit(0);
                }
                break;
            default:
                if (section != 2)
                {
                    warnx("Unrecognized option");
                    errx(1, "Usage: ./42sh [options] [file]");
                }
        }
    }
}
