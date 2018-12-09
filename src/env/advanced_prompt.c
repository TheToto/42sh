/**
* @file advanced_prompt.c
* @author louis.holleville
* @date 05-12-2018
* @version 0.9
* @brief Replacement of PS1 & PS2 by their real value
*/

#define _DEFAULT_SOURCE
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "shell.h"
#include "env.h"

static char *my_realloc(char *ptr, size_t *capacity)
{
    char *new = realloc(ptr, 2 * sizeof(char) * *capacity);
    if (!new)
        err(1, "customize_str: cannot realloc new PSN");
    *capacity *= 2;
    ptr = new;
    return new;
}

static char *get_d_time(char *new, size_t *size, size_t *capacity,
                        char *format)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);

    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, format, timeinfo);
    for (size_t i = 0; buffer[i]; i++)
    {
        new[*size] = buffer[i];
        *size += 1;
        if (*size >= *capacity - 1)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *get_D_time(char *old, char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    size_t *i = arr[2];
    size_t j = 0;
    *i += 1;
    if (old[*i] && (old[*i] == '{') && old[*i + 1])
        *i += 2;
    else
    {
        free(new);
        return NULL;
    }
    char buffer[1024];
    while (old[*i] != '}')
    {
        if (*size == *capacity)
            new = my_realloc(new, capacity);
        buffer[j++] = old[*i];
        *i += 1;
        if (!old[*i])
        {
            free(new);
            return NULL;
        }
    }
    buffer[j] = 0;
    return get_d_time(new, size, capacity, buffer);
}

static char *get_W(char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    char *path = get_var(shell.var, "PWD");
    if (!path)
    {
        free(new);
        return NULL;
    }
    path = basename(path);
    for (size_t j = 0; path[j]; j++)
    {
        new[*size] = path[j];
        *size += 1;
        if (*size == *capacity)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *get_H(char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    char path[HOST_NAME_MAX] =
    {
        0
    };
    if (gethostname(path, HOST_NAME_MAX) == -1)
    {
        free(new);
        return NULL;
    }
    for (size_t j = 0; path[j]; j++)
    {
        new[*size] = path[j];
        *size += 1;
        if (*size == *capacity)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *get_h(char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    char path[HOST_NAME_MAX] =
    {
        0
    };
    if (gethostname(path, HOST_NAME_MAX) == -1)
    {
        free(new);
        return NULL;
    }
    char *tmp = strtok(path, ".");
    if (tmp != path)
        *(tmp - 1) = 0;

    for (size_t j = 0; path[j]; j++)
    {
        new[*size] = path[j];
        *size += 1;
        if (*size == *capacity)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *get_u(char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    char *path = get_var(shell.var, "USER");
    if (!path)
    {
        free(new);
        return NULL;
    }
    for (size_t j = 0; path[j]; j++)
    {
        new[*size] = path[j];
        *size += 1;
        if (*size == *capacity)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *get_w(char *new, size_t **arr)
{
    size_t *size = arr[0];
    size_t *capacity = arr[1];
    char *path = get_var(shell.var, "PWD");
    if (!path)
    {
        free(new);
        return NULL;
    }
    for (size_t j = 0; path[j]; j++)
    {
        new[*size] = path[j];
        *size += 1;
        if (*size == *capacity)
            new = my_realloc(new, capacity);
    }
    return new;
}

static char *my_err(void)
{
    char *save = calloc(12, sizeof(char));
    if (!save)
        err(1, "customize_str: cannot calloc err string");
    save[0] = '[';
    save[1] = '4';
    save[2] = '2';
    save[3] = 's';
    save[4] = 'h';
    save[5] = '@';
    save[6] = 'p';
    save[7] = 'c';
    save[8] = ']';
    save[9] = '$';
    save[10] = ' ';
    return save;
}

static char *customize_str2(char *new, char *old, size_t **arr)
{
    size_t *size = arr[0];
    size_t *i = arr[2];
    switch (old[*i])
    {
    case 'e':
        new[*size] = 33;
        *size += 1;
        return new;
    case 'n':
        new[*size] = 10;
        *size += 1;
        return new;
    case 'r':
        new[*size] = 15;
        *size += 1;
        return new;
    case '\\':
        new[*size] = '\\';
        *size += 1;
        return new;
    default:
        new[*size] = old[*i];
        *size += 1;
    }
    return new;
}

static char *customize_str(char *new, char *old, size_t size, size_t capacity)
{
    for (size_t i = 0; old[i] && new; i++)
    {
        if (size >= capacity - 1)
            new = my_realloc(new, &capacity);
        if (old[i] == '\\')
        {
            if (!old[i++])
                err(1, "FATAL ERROR: PSN cannot be of this format at all");
            size_t *arr[3] =
            {
                &size, &capacity, &i
            };
            switch (old[i])
            {
            case 'a':
                new[size++] = 7;
                break;
            case 'd':
                new = get_d_time(new, &size, &capacity, "%a %b %d");
                break;
            case 'D':
                new = get_D_time(old, new, arr);
                break;
            case 'W':
                new = get_W(new, arr);
                break;
            case 'w':
                new = get_w(new, arr);
                break;
            case 'u':
                new = get_u(new, arr);
                break;
            case 'H':
                new = get_H(new, arr);
                break;
            case 'h':
                new = get_h(new, arr);
                break;
            default:
                new = customize_str2(new, old, arr);
            }
        }
        else
            new[size++] = old[i];
    }
    if (!new)
        return my_err();
    return new;
}

char *advanced_prompt(char *psn)
{
    char *old = get_var(shell.var, psn);
    char *new = calloc(256, sizeof(char));
    if (!new)
        err(1, "advanced_prompt: cannot calloc new psn string");
    size_t size = 0;
    size_t capacity = 256;
    new = customize_str(new, old, size, capacity);
    add_var(shell.var, "$RESERVED_PSN", new, 0);
    free(new);
    return get_var(shell.var,"$RESERVED_PSN");
}
