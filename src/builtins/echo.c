/**
* \file echo.c
* \brief Execute the echo builtin
* \author sabrina.meng louis.holleville
* \version 1.0
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
        for (size_t j = 1; str[i][j]; j++)
        {
            if (str[i][1])
            {
                switch (str[i][j])
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
    }
    return n + e + E;
}

static int not_opt(char *str)
{
    if (str[0] != '-')
        return 1;
    for (size_t i = 1; str[i]; i++)
    {
        if (str[i] != 'n' && str[i] != 'e' && str[i] != 'E')
            return 1;
    }
    return 0;
}

static int not_hexa(char c)
{
    return !(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f')
        && !(c >= 'A' && c <= 'F');
}

static void convert_dec(char *str, char *to_print, size_t *old, size_t *new)
{
    char n[3] =
    {
        0
    };
    size_t base = str[*old] == '0' ? 8 : 16;
    char *res = calloc(3, sizeof(char));
    if (!res)
    {
        warnx("Calloc failed in convert_dec");
        return;
    }
    if (str[*old] == '0')
    {
        for (size_t i = 1; i <= 3; i++)
        {
            if (!(str[*old + i] >= '0' && str[*old + i] <= '7'))
                break;
            n[i - 1] = str[*old + i];
        }

    }
    else if (str[*old] == 'x')
    {
        for (size_t i = 1; i <= 2; i++)
        {
            if (not_hexa(str[*old + i]))
                break;
            n[i - 1] = str[*old + i];
        }
    }
    if (!n[0])
    {
        *new += 1;
        to_print[*new - 1] = str[*old - 1];
        to_print[*new] = str[*old];
        return;
    }
    int c = strtol(n, NULL, base);
    sprintf(res, "%c", c);
    to_print[*new] = res[0];
    *old += strlen(n);
    free(res);
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
        to_print[*new] = 8;
        break;
    case 'e':
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
    case '0':
    case 'x':
        convert_dec(str, to_print, old, new);
        break;
    default:
        *new += 1;
        to_print[*new - 1] = str[*old - 1];
        to_print[*new] = str[*old];
        break;
    }
}

static void my_printf(char *to_print, char **str, size_t i)
{
    printf("%s", to_print);
    if (str[i + 1])
        printf(" ");
    free(to_print);
}

int echo(char **str)
{
    char flags = get_flags(str);
    flags = (((flags & 4) == 4) && ((flags & 2) == 2)) ? flags - 2 : flags;
    int in_opt = 1;
    for (size_t i = 1; str[i]; i++)
    {
        if (in_opt)
            in_opt = !not_opt(str[i]);
        if (!in_opt)
        {
            int len = !strlen(str[i]) ? 1 : strlen(str[i]) * 2;
            char *to_print = calloc(len, sizeof(char));
            size_t old = 0;
            size_t new = 0;
            for (; str[i][old]; old++, new++)
            {
                if (((flags & 2) == 2) && str[i][old] == '\\')
                {
                    if (str[i][old + 1] && str[i][old + 1] == 'c')
                    {
                        flags += (flags & 1) == 0 ? 1 : 0;
                        break;
                    }
                    handle_escape(str[i], to_print, &old, &new);
                }
                else
                    to_print[new] = str[i][old];
            }
            my_printf(to_print, str, i);
        }
    }
    if ((flags & 1) == 0)
        puts("");
    fflush(stdout);
    return 0;
}
