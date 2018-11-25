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

int launch_file(char *path, int is_print, struct variables *var)
{
    FILE *f = fopen(path, "r");
    if (!f)
        err(1, "can't open %s", path);

    fseek(f, 0L, SEEK_END);
    size_t numbytes = ftell(f);

    fseek(f, 0L, SEEK_SET);
    char *buffer = calloc(numbytes + 1, sizeof(char));

    if(buffer == NULL)
        err(1, "Failed to malloc buffer");

    fread(buffer, sizeof(char), numbytes, f);
    fclose(f);
    int res = exec_main(buffer, is_print, var);

    free(buffer);
    return res;
}

int launch_pipe(int is_print)
{
    int i = 0;
    char pipe[65535];
    for (; i < 65536; i++)
    {
        char tmp = getc(stdin);
        if (tmp == EOF)
            break;
        pipe[i] = tmp;
    }
    pipe[i] = '\0';
    struct variables *library = init_var();
    int res = exec_main(pipe, is_print, library);
    destroy_var(library);
    return res;
}
