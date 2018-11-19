/**
 *\file readfile.c
 *\author thomas.lupin
 *\version 0.3
 *\date 15-11-2018
 *\brief Read a file
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "execution.h"

/**
 *\fn launch_file
 *\brief Read the whole file \b path and send it to exec
 *\param path Path to file
 */
void launch_file(char *path, int is_print)
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
    exec_main(buffer, is_print);

    free(buffer);
}
