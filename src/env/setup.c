/**
* \file setup.c
* \author louis.holleville
* \version 1.0
* \date 10-12-2018
* \brief Management of pre-loaded data
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fnmatch.h>

#include "builtins.h"
#include "env.h"
#include "ast.h"
#include "ast_destroy.h"
#include "shell.h"
#include "shopt.h"
#include "quote_lexer.h"
#include "shopt.h"
#include "maths.h"
#include "queue.h"
#include "pathexp.h"
#include "subshell.h"

extern char **environ;

static void *my_realloc(void *p, size_t *size)
{
    char *ptr = realloc(p, 2 * *size);
    if (!ptr)
        err(1, "cannot realloc in my_realloc");
    p = ptr;
    *size *= 2;
    return p;
}

static char *itoa(int i, char *buf_nb)
{
    sprintf(buf_nb, "%d", i);
    return buf_nb;
}

void set_up_reserved(void)
{
    char buf_nb[20] =
    {
        0
    };
    add_var(shell.var, "$", itoa(getpid(), buf_nb), 0);
    add_var(shell.var, "UID", itoa(getuid(), buf_nb), 0);
    char *pwd = get_current_dir_name();
    add_var(shell.var, "OLDPWD", "", 0);
    add_var(shell.var, "PWD", pwd, 0);
    free(pwd);
    add_var(shell.var, "RANDOM", "32767", 0);
    add_var(shell.var, "?", "", 0);
    add_var(shell.var, "PS1", "[\\u@\\h \\W ]$ ", 0);
    add_var(shell.var, "PS2", "> ", 0);

    if (!get_var(shell.var, "HOME"))
        add_var(shell.var, "HOME", getenv("HOME"), 0);
    shell.shopt_states = init_shoptlist();
    update_shellopts();
    add_var(shell.var, "IFS", " \t\n", 0);
}
void set_up_var(char *args[])
{
    char buf_nb[20] =
    {
        0
    };
    int nb = 0;
    int size = 0;
    for (; args && args[nb] && nb < 100; nb++)
    {
        add_var(shell.var, itoa(nb, buf_nb), args[nb], 0);
        size += strlen(args[nb]) + 1;
    }
    add_var(shell.var, "#", itoa(nb - 1, buf_nb), 0);
    // $@ $*
    char *star = calloc(size + 1, sizeof(char));
    for (size_t i = 1; args && args[0] && args[i]; i++)
    {
        strcat(star, args[i]);
        if (args[i + 1])
            strcat(star, " ");
    }
    add_var(shell.var, "*", star, 0);
    add_var(shell.var, "@", star, 0);

    free(star);
}

void import_exported(struct variables *var)
{
    for (size_t i = 0; environ[i]; i++)
    {
        size_t size = 255;
        char *name = calloc(255, sizeof(char));
        size_t j = 0;
        for (; environ[i][j] && environ[i][j] != '='; j++)
        {
            name[j] = environ[i][j];
            if (i >= size - 1)
                name = my_realloc(name, &size);
        }
        name[j] = '\0';
        char *value = calloc(255, sizeof(char));
        size = 255;
        if (environ[i][j])
            j++;

        size_t k = 0;
        for (; environ[i][j]; j++, k++)
        {
            value[k] = environ[i][j];
            if (k >= size - 1)
                value = my_realloc(value, &size);
        }

        value[k] = '\0';
        add_var(var, name, value, 1);

        free(name);
        free(value);
    }
}
