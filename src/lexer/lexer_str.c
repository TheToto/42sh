/**
* \file lexer_str.c
* \brief Contain all function to get next string until a
* space or tab in a string.
* \author Arthur Busuttil
* \version 0.8
*/
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"
#include "shell.h"

static void get_next_subshell(char *str, int *cur)
{
    int lvl = 1;
    int i = *cur;
    if (!str[i + 1] || !str[i + 2])
        return;
    i += 2;
    while (str[i] != ')' && lvl)
    {
        lvl += (str[i] == '(') ? 1 : (str[i] == ')') ? -1 : 0;
        i++;
    }
    *cur = i;
}

static int get_next_quoted(char *str)
{
    int i = 0;
    for (; str[i] && str[i] != '\t' && str[i] != ' '; i++)
    {
        if (str[i] == '$' && str[i + 1] == '(')
            get_next_subshell(str, &i);
        else if (str[i] == '\'' && !(i > 0 && str[i - 1] == '\\'))
        {
            i++;
            while (str[i] && (str[i] != '\''))
                i++;
        }
        else if (str[i] == '\"' && !(i > 0 && str[i - 1] == '\\'))
        {
            i++;
            while (str[i]
                && (str[i] != '\"' || (i > 0 && str[i - 1] == '\\')))
                i++;
        }
        else if (str[i] == '`' && !(i > 0 && str[i - 1] == '\\'))
        {
            i++;
            while (str[i]
                && (str[i] != '`' || (i > 0 && str[i - 1] == '\\')))
                i++;
        }
    }
    return i;
}

static void skip_space_and_tab(char **beg)
{
    for (; **beg && (**beg == ' ' || **beg == '\t'); (*beg)++)
        continue;
}

static void skip_comment(char **beg)
{
    if (**beg == '#')
    {
        for (; **beg && **beg != '\n'; (*beg)++)
            continue;
    }
}

char *get_next_str(char **beg, char **ptr)
{
    if (!beg || !*beg || !**beg)
        return NULL;
    size_t len = 0;
    skip_space_and_tab(beg);
    skip_comment(beg);
    *ptr = *beg;
    len = get_next_quoted(*beg);
    char *res = calloc(1, len + 1);
    if (res)
    {
        strncpy(res, *beg, len);
        if (!strlen(*beg))
        {
            free(res);
            return NULL;
        }
        *beg += len;
    }
    return res;
}
