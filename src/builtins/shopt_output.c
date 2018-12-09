/**
* \file shopt_output.c
* \brief Handle the output of the [-+]O option and the shopt builtin
* \authors sabrina.meng
* \version 0.9
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
    for (size_t i = 0; i < NB_SHOPT; i++)
        shell.shopt_states[i] = 0;
    shell.shopt_states[SRCPATH] = 1;
    shell.shopt_states[EXP_ALIAS] = 1;
    return shell.shopt_states;
}

static void reset_shopt(enum shopt shopt)
{
    switch (shopt)
    {
    case ASTPRINT:
    case DOTGLOB:
    case EXTGLOB:
    case NOCASEGLOB:
    case NULLGLOB:
    case XPGECHO:
        shell.shopt_states[shopt] = 0;
        break;
    case EXP_ALIAS:
    case SRCPATH:
        shell.shopt_states[shopt] = 1;
        break;
    default:
        break;
    }
}

int err_shopt(void)
{
    char *msg1 = "Invalid arguments for [-+]O option\n";
    char *msg2 = "    Usage: [-+]O shopt_variable\n";
    char *msg3 = "    Shopt variables:\n";
    char *msg4 = "        ast_print\n        dotglob\n";
    char *msg5 = "        expand_aliases\n        extglob\n";
    char *msg6 = "        nocaseglob\n        nullglob\n";
    char *msg7 = "        sourcepath\n        xpg_echo";
    if (shell.type == S_PROMPT)
        warnx("%s%s%s%s%s%s%s", msg1, msg2, msg3, msg4, msg5, msg6, msg7);
    else
        errx(2, "%s%s%s%s%s%s%s", msg1, msg2, msg3, msg4, msg5, msg6, msg7);
    return 2;
}

void print_shopt(int is_builtin, enum shopt shopt)
{
    if (!is_builtin && shopt == NO)
        init_shoptlist();
    size_t i = 0;
    if (shopt == NO)
    {
        printf("ast_print      \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("dotglob        \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("expand_aliases \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("extglob        \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("nocaseglob     \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("nullglob       \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("sourcepath     \t%s\n",
                shell.shopt_states[i++] ? "on" : "off");
        printf("xpg_echo       \t%s\n",
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
