/**
* \file shopt.c
* Handle the shopt variables
* \authors sabrina.meng
* \version 0.8
* \date 28-11-2018
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "shopt.h"

static struct shopt_state shopt_list[NB_SHOPT];

static void init_list(void)
{
    size_t j = ASTPRINT;
    for (size_t i = 0; i < NB_SHOPT; i++, j++)
    {
        shopt_list[i].shopt = j;
        shopt_list[i].set = 0;
    }
    shopt_list[SRCPATH - 2].set = 1;
}

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

void err_shopt(void)
{
    errx(1, "Invalid arguments for [-+]O option\n\
      Usage: [-+]O shopt_variable\n\
      Shopt variables:\n\
            ast_print\n\
            dotglob\n\
            expand_aliases\n\
            extglob\n\
            nocaseglob\n\
            nullglob\n\
            sourcepath\n\
            xpg_echo");
}

void print_shopt(int is_builtin)
{
    if (!is_builtin)
        init_list();
    size_t i = 0;
    printf("ast_print         %s\n", shopt_list[i++].set ? "on" : "off");
    printf("dotglob           %s\n", shopt_list[i++].set ? "on" : "off");
    printf("expand_aliasases  %s\n", shopt_list[i++].set ? "on" : "off");
    printf("extglob           %s\n", shopt_list[i++].set ? "on" : "off");
    printf("nocaseglob        %s\n", shopt_list[i++].set ? "on" : "off");
    printf("nullglob          %s\n", shopt_list[i++].set ? "on" : "off");
    printf("sourcepath        %s\n", shopt_list[i++].set ? "on" : "off");
    printf("xpg_echo          %s\n", shopt_list[i++].set ? "on" : "off");
}

void print_shopt_plus(void)
{
    printf("shopt -u ast_print\n");
    printf("shopt -u dotglob\n");
    printf("shopt -u expand_aliases\n");
    printf("shopt -u extglob\n");
    printf("shopt -u nocaseglob\n");
    printf("shopt -u nullglob\n");
    printf("shopt -s sourcepath\n");
    printf("shopt -u xpg_echo\n");
}
