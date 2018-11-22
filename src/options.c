/**
 *\file options.c
 *\author sabrina.meng
 *\version 0.3
 *\date 15-11-2018
 *\brief Options parsing
 *\detail Parse the options with the format
 *[GNU long option] [option] script-file
 */

#define _POSIX_C_SOURCE
#define _DEFAULT_SOURCE

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>

#include "options.h"
#include "lexer.h"
#include "print.h"
#include "execution.h"
#include "ast_destroy.h"
#include "var.h"

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
enum option get_option(char *opt)
{
    if (!strcmp(opt, "--norc"))
        return NORC;
    if (!strcmp(opt, "--ast-print"))
        return AST;
    if (!strcmp(opt, "--version"))
        return VERSION;
    if (!strcmp(opt, "-c"))
        return CMD;
    if (!strcmp(opt, "-O"))
        return SHOPT_MINUS;
    if (!strcmp(opt, "+O"))
        return SHOPT_PLUS;
    return NONE;
}

/**
 *\fn get_shopt
 *\brief Get the shopt variable according to the enum shopt of the header
 *\param char *arg  The shopt variable to check
 *\return Return an enum value according to the shopt variable
 */
enum shopt get_shopt(char *arg)
{
    if (!arg)
        return NO;
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
 *\fn print_shopt_minus
 *\brief Prints the same output as bash with the -O option
 */
static void print_shopt_minus(void)
{
    printf("ast_print         off\n");
    printf("dotglob           off\n");
    printf("expand_aliasases  off\n");
    printf("extglob           off\n");
    printf("nocaseglob        off\n");
    printf("nullglob          off\n");
    printf("sourcepath        on\n");
    printf("xpg_echo          off\n");
    exit(0);
}

/**
 *\fn print_shopt_plus
 *\brief Prints the same output as bash with the +O option
 */
static void print_shopt_plus(void)
{
    printf("shopt -u ast_print\n");
    printf("shopt -u dotglob\n");
    printf("shopt -u expand_aliases\n");
    printf("shopt -u extglob\n");
    printf("shopt -u nocaseglob\n");
    printf("shopt -u nullglob\n");
    printf("shopt -s sourcepath\n");
    printf("shopt -u xpg_echo\n");
    exit(0);
}

static int check_ast_print(char **argv)
{
    for (size_t i = 1; argv[i]; i++)
    {
        if (!strcmp(argv[i], "--ast-print"))
            return 1;
    }
    return 0;
}

static void exec_shopt(char **argv, size_t *i, size_t section, enum option opt)
{
    if (section == 1)
    {
        (*i)++;
        enum shopt shopt = get_shopt(argv[*i]);
        if (shopt == OTHER)
            err_shopt();
        if (shopt == NO && opt == SHOPT_MINUS)
            print_shopt_minus();
        if (shopt == NO && opt == SHOPT_PLUS)
            print_shopt_plus();
    }
}

static void exec_cmd(size_t section, char **argv, size_t i, int ast)
{
    if (section == 1 && !(argv[++i] && argv[i][0] != '-'))
    {
        warnx("Invalid arguments for -c option");
        errx(1, "Usage: -c <command>");
    }
    struct variables *library = init_var();
    int res = exec_main(argv[i], ast, library);
    destroy_var(library);
    exit(res);
}

/**
 *\fn options
 *\brief Do actions according to each options
 *\param char *argv[]   The command line to parse
 *\return The return value depends on the options
 */
void options(char *argv[])
{
    size_t section = 0;
    size_t i = 1;
    int ast = check_ast_print(argv);
    int norc = 0;
    for ( ; argv[i]; i++)
    {
        size_t sect = get_section(argv[i]);
        section = section > sect ? section : sect;
        if (section == 2)
            break;
        enum option opt = get_option(argv[i]);
        if (opt == CMD)
            exec_cmd(section, argv, i, ast);
        else if (opt == SHOPT_PLUS || opt == SHOPT_MINUS)
            exec_shopt(argv, &i, section, opt);
        else if (opt == NORC) //if (!section) : deactivate ressource reader
            norc = 1;
        else if (opt == AST) //OK
            continue;
        else if (opt == VERSION)
        {
            if (!section)
            {
                printf("Version 0.3\n");
                exit(0);
            }
        }
    }
    if (!argv[i])
    {
        if (isatty(STDIN_FILENO))
        {
            exit(show_prompt(norc, ast));
        }
        else
        {
            errx(1, "Handle pipe here");
        }
    }
    else
    {
        int res = 0;
        for ( ; argv[i]; i++)
        {
            printf("File to exec : %s\n", argv[i]);
            res = launch_file(argv[i], ast);
        }
        exit(res);
    }
}
