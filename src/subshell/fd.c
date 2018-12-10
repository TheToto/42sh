/**
* @file fd.c
* @author louis.holleville
* @date 07-12-2018
* @version 1.0
* \brief Management of fd in subshells
*/

#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char *my_recalloc(char *p, size_t *capacity, size_t *size)
{
    char *new = realloc(p, 2 * sizeof(char) * *capacity);
    if (!new)
        err(1, "subshell: fd_to_string: cannot recalloc");
    *capacity *= 2;
    for (size_t i = *size; i < *capacity; i++)
        new[i] = 0;
    p = new;
    return new;
}


char *fd_to_string(int fd)
{
    size_t size = 0;
    size_t capacity = 255;
    char *res = calloc(256, sizeof(char));
    if (!res)
        err(1, "subshell: fd_to_string: cannot calloc");
    lseek(fd, 0, SEEK_SET);
    char tmp[2] =
    {
        0
    };
    while (read(fd, tmp, 1))
    {
        res[size++] = tmp[0];
        if (size == capacity)
            res = my_recalloc(res, &capacity, &size);
    }
    if (strlen(res) > 1)
        res[size - 1] = '\0';
    res[size] = '\0';
    close(fd);
    return res;
}
