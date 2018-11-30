/**
* \file shopt_output.c
* Handle the output of the [-+]O option and the shopt builtin
* \authors sabrina.meng
* \version 0.8
* \date 28-11-2018
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "shopt.h"
#include "shell.h"

int *init_shoptlist(void)
{
    if (!shell.shopt_states)
    {
        shell.shopt_states = malloc(sizeof(int) * NB_SHOPT + 1);
        if (!shell.shopt_states)
            errx(1, "Malloc error");
    }
    size_t j = ASTPRINT;
    for (size_t i = 0; i < NB_SHOPT; i++, j++)
        shell.shopt_states[i] = 0;
    shell.shopt_states[SRCPATH - 2] = 1;
    return shell.shopt_states;
}

static void reset_shopt(enum shopt shopt)
{
    switch (shopt)
    {
    case ASTPRINT:
    case DOTGLOB:
    case EXP_ALIAS:
    case EXTGLOB:
    case NOCASEGLOB:
    case NULLGLOB:
    case XPGECHO:
        shell.shopt_states[shopt - 2] = 0;
        break;
    case SRCPATH:
        shell.shopt_states[shopt - 2] = 1;
        break;
    default:
        break;
    }
}

void err_shopt(void)
{
    char *msg = "Invalid arguments for [-+]O option\n\
      Usage: [-+]O shopt_variable\n\
      Shopt variables:\n\
            ast_print\n\
            dotglob\n\
            expand_aliases\n\
            extglob\n\
            nocaseglob\n\
            nullglob\n\
            sourcepath\n\
            xpg_echo";
    if (shell.type == S_PROMPT)
        warnx(msg);
    else
        errx(2, msg);
}

void print_shopt(int is_builtin, enum shopt shopt)
{
    if (!is_builtin && shopt == NO)
        init_shoptlist();
    size_t i = 0;
    if (shopt == NO)
    {
        printf("ast_print       %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("dotglob         %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("expand_aliases  %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("extglob         %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("nocaseglob      %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("nullglob        %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("sourcepath      %s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("xpg_echo        %s\n",
                shell.shopt_states[i++] ? "on" : "off");
    }
    else
        reset_shopt(shopt);
}

void print_shopt_plus(enum shopt shopt)
{
    if (shopt == NO)
    {
        init_shoptlist();
        printf("shopt -u ast_print\n");
        printf("shopt -u dotglob\n");
        printf("shopt -u expand_aliases\n");
        printf("shopt -u extglob\n");
        printf("shopt -u nocaseglob\n");
        printf("shopt -u nullglob\n");
        printf("shopt -s sourcepath\n");
        printf("shopt -u xpg_echo\n");
    }
    else
        reset_shopt(shopt);
}
