/**
* \file readfile.c
* \author thomas.lupin
* \version 0.5
* \date 15-11-2018
* \brief Read a file
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "execution.h"
#include "shell.h"

int launch_file(char *path, int is_print, struct variables *var)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        if (shell.type != S_PROMPT)
            err(1, "can't open %s", path);
        else
        {
            warn("can't open %s", path);
            return 1;
        }
    }

    fseek(f, 0L, SEEK_END);
    size_t numbytes = ftell(f);

    fseek(f, 0L, SEEK_SET);
    char *buffer = calloc(numbytes + 1, sizeof(char));

    if (buffer == NULL)
        err(1, "Failed to malloc buffer");

    fread(buffer, sizeof(char), numbytes, f);
    fclose(f);
    shell.buf = buffer;
    int res = exec_main(buffer, is_print, var);
    free(buffer);
    shell.buf = NULL;
    return res;
}

int launch_pipe(int is_print)
{
    size_t capacity = 255;
    size_t size = 0;
    char *pipe = malloc(sizeof(char) * 255);
    shell.buf = pipe;
    for (int i = 0; 1; i++)
    {
        char tmp = getc(stdin);
        if (tmp == EOF)
            break;
        size++;
        pipe[i] = tmp;
        if (size == capacity - 1)
        {
            char *check = realloc(pipe, capacity * 2);
            if (!check)
                err(1, "Failed to realloc pipe");
            pipe = check;
            shell.buf = pipe;
            capacity *= 2;
        }
    }
    pipe[size] = '\0';
    int res = exec_main(pipe, is_print, shell.var);
    free(pipe);
    shell.buf = NULL;
    return res;
}
