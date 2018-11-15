#include "options.h"

static size_t get_section(char *str)
{
    switch (str[0])
    {
        case '-':
            if (str[1] && str[1] == '-')
                return 0;
            else
                return 1;
        default:
            return 2;
    }
}

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
    }
    else if (section == 1)
    {
        if (!strcmp(opt, "-c"))
            return CMD;
    }
    return NONE;
}

void check_options(char *argv[])
{
    enum option opt = NONE;
    size_t section = 0;

    for (size_t i = 1; argv[i]; i++)
    {
        size_t sect = get_section(argv[i]);
        section = section > sect ? section : sect;
        opt = get_option(argv[i], section);
        switch (opt)
        {
            case CMD:
                if (argv[++i] && argv[i][0] != '-')
                    printf("-c OK\n");
                    //bash behavior
                else
                {
                    warnx("Invalid arguments for -c option");
                    errx(1, "Usage: -c <command>");
                }
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
                if (section == 2)
                    printf("A file\n");
                else
                {
                    warnx("Unrecognized option");
                    errx(1, "Usage: ./42sh [GNU long options] [options] [file]");
                }
        }
    }
}
