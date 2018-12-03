/**
 * \file echo.c
 * Execute the echo builtin
 * \author sabrina.meng louis.holleville
 * \version 0.8
 * \date 02-12-2018
**/

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include "builtins.h"

static char get_flags(char **str)
{
    char n = 0;
    char e = 0;
    char E = 0;
    for (size_t i = 1; str[i] && str[i][0] == '-'; i++)
    {
        if (str[i][1] && !str[i][2])
        {
            switch (str[i][1])
            {
                case 'n':
                    n = 1;
                    break;
                case 'e':
                    e = 2;
                    break;
                case 'E':
                    E = 4;
                    break;
                default:
                    return n + e + E;
            }
        }
        else
            return n + e + E;
    }
    return n + e + E;
}

static int not_opt(char *str)
{
    return !(str[0] == '-' && str[1] && !str[2] && (str[1] == 'n'
                || str[1] == 'e' || str[1] =='E'));
}


static void handle_b(size_t *new)
{
    if (*new > 0)
    {
        *new -= 1;
    }
}

static void handle_escape(char *str, char *to_print, size_t *old, size_t *new)
{
    *old += 1;
    switch (str[*old])
    {
        case 'a':
            to_print[*new] = 7;
            break;
        case 'b':
            handle_b(new);
            break;
        case 'e':
            to_print[*new] = 27;
            break;
        case 'E':
            to_print[*new] = 27;
            break;
        case 'f':
            to_print[*new] = 12;
            break;
        case 'n':
            to_print[*new] = '\n';
            break;
        case 'r':
            to_print[*new] = 13;
            break;
        case 't':
            to_print[*new] = 9;
            break;
        case 'v':
            to_print[*new] = 11;
            break;
        case '\\':
            to_print[*new] = '\\';
            break;
        default:
            //ask PS2=
            break;
    }
}

int echo(char **str)
{
    char flags = get_flags(str);
    flags = (((flags & 4) == 4) && ((flags & 2) == 2)) ? flags - 2 : flags;
    for (size_t i = 1; str[i]; i++)
    {
        if (not_opt(str[i]))
        {
            char *to_print = calloc(strlen(str[i]) + 2, sizeof(char));
            size_t old = 0;
            size_t new = 0;
            for (; str[i][old]; old++, new++)
            {
                if (((flags & 2) == 2) && str[i][old] == '\\')
                {
                    if (str[i][old + 1] && str[i][old] == 'c')
                    {
                       flags += (flags & 1) == 0 ? 1: 0;
                       break;
                    }
                    handle_escape(str[i], to_print, &old, &new);
                }
                else
                    to_print[new] = str[i][old];
            }
            printf("%s", to_print);
            free(to_print);
        }
    }
    if ((flags & 1) == 0)
        puts("");
    fflush(stdout);
    return 0;
}
