/**
* \file options.c
* Parse the options with the format [GNU long option] [option] script-file
* \authors sabrina.meng thomas.lupin
* \version 0.8
* \date 27-11-2018
*/

#define _POSIX_C_SOURCE
#define _DEFAULT_SOURCE

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include "options.h"
#include "shopt.h"
#include "lexer.h"
#include "print.h"
#include "execution.h"
#include "ast_destroy.h"
#include "shell.h"
#include "env.h"
#include "readfile.h"
#include "builtins.h"

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

static int check_ast_print(char **argv)
{
    if (shell.shopt_states[ASTPRINT])
        return 1;
    for (size_t i = 1; argv[i]; i++)
    {
        if (!strcmp(argv[i], "--ast-print"))
            return 1;
    }
    return 0;
}

static void exec_shopt(char **argv, size_t i, enum option opt)
{
    enum shopt shopt = get_shopt(argv[i + 1]);
    if (shopt == OTHER)
        err_shopt();
    if (shopt == NO && opt == SHOPT_MINUS)
        print_shopt(0, shopt);
    if (shopt == NO && opt == SHOPT_PLUS)
        print_shopt_plus(shopt);
}

static void exec_cmd(char **argv, size_t i, int ast)
{
    if (!(argv[++i] && argv[i][0] != '-'))
    {
        errx(1, "Invalid arguments for -c option\n\
          Usage: -c <command>");
    }
    shell.type = S_OPTION;
    struct variables *library = init_var();
    set_up_var(argv);
    int res = exec_main(argv[i], ast, library);
    destroy_var(library);
    exit(res);
}

static void launch_sh(char *argv[], int i, int ast, int norc)
{
    struct variables *var = init_var();
    int res = 0;
    if (!argv[i])
    {
        if (isatty(STDIN_FILENO))
        {
            shell.type = S_PROMPT;
            set_up_var(argv);
            res = show_prompt(norc, ast);
        }
        else
        {
            set_up_var(argv);
            shell.type = S_INPUT;
            res = launch_pipe(ast);
        }
    }
    else
    {
        shell.type = S_FILE;
        set_up_var(argv + 1);
        res = launch_file(argv[1], ast, var);
    }
    destroy_var(var);
    exit(res);
}

void options(char *argv[])
{
    size_t i = 1;
    shell.shopt_states = init_shoptlist();
    int ast = check_ast_print(argv);
    int norc = 0;

    for (; argv[i]; i++)
    {
        if (argv[i][0] != '-' && argv[i][0] != '+')
            break;
        enum option opt = get_option(argv[i]);
        if (opt == CMD)
            exec_cmd(argv, i, ast);
        else if (opt == CMD || opt == AST)
            continue;
        else if (opt == SHOPT_MINUS || opt == SHOPT_PLUS)
            exec_shopt(argv, i, opt);
        else if (opt == NORC)
            norc = 1;
        else if (opt == VERSION)
        {
                printf("Version 0.5\n");
                exit(0);
        }
    }
    launch_sh(argv, i, ast, norc);
}
