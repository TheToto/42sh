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
#include "lexer.h"
#include "print.h"
#include "execution.h"
#include "ast_destroy.h"

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
    if (arg[0] == '+')
        return 1;
    return 2;
}

/**
 *\fn get_option
 *\brief Get the option type according to the enum option of the header
 *\param char *opt  The option to check
 *\return Return an enum value according to the option
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
 *\fn get_shopt
 *\brief Get the shopt variable according to the enum shopt of the header
 *\param char *arg  The shopt variable to check
 *\return Return an enum value according to the shopt variable
 */
static enum shopt get_shopt(char *arg)
{
    if (!arg)
        return OTHER;
    if (!strcmp(arg, "ast_print"))
        return ASTPRINT;
    if (!strcmp(arg, "dotglob"))
        return DOTGLOB;
    if (!strcmp(arg, "expand_aliases"))
        return EXP_ALIAS;
    if (!strcmp(arg, "extglob"))
        return EXTGLOB;
    if (!strcmp(arg, "nocaseglob"))
        return NOCASEGLOB;
    if (!strcmp(arg, "nullglob"))
        return NULLGLOB;
    if (!strcmp(arg, "sourcepath"))
        return SRCPATH;
    if (!strcmp(arg, "xpg_echo"))
        return XPGECHO;
    return OTHER;
}

/**
 *\fn err_shopt
 *\brief Prints an error message on stderr for the [+]O option
 */
static void err_shopt(void)
{
    warnx("Invalid arguments for [-+]O option");
    warnx("Usage: [-+]O shopt_variable");
    errx(1, "Shopt variables:\n\
    \tast_print\n\
    \tdotglob\n\
    \texpand_aliases\n\
    \textglob\n\
    \tnocaseglob\n\
    \tnullglob\n\
    \tsourcepath\n\
    \txpg_echo");
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
                printf("exec: %s\n", argv[i]);
                struct lexer *l = lexer(argv[i]);
                struct token_list *copy = l->token_list;
                struct ast_node *ast = rule_input(&(l->token_list));
                l->token_list = copy;
                makedot(ast, "ast.dot");

                printf("\nExecution result:\n");
                exec_node(ast);
                destroy_ast(ast);
                lexer_destroy(l);
                break;
            case SHOPT:
                if (section == 1)
                {
                    enum shopt shopt = get_shopt(argv[++i]);
                    if (shopt == OTHER)
                        err_shopt();
                }
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
