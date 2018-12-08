/**
* @file subshell.c
* @author thomas.lupin louis.holleville
* @date 07-12-2018
* @version 0.9
* Management of subshells
*/

#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ast.h"
#include "subshell.h"
#include "execution.h"

static int lauch_subshell(void)
{
    return 0;
}

int redirect_subshell(char *input, char *output)
{
    *input = 0;//avoiding warning TO BE REMOVE
    *output = 0;//avoiding warning TO BE REMOVE
    int fd = open("/tmp", O_TMPFILE | O_RDWR);
    int save = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    int res = lauch_subshell();

    dup2(save, STDOUT_FILENO);
    output = fd_to_string(fd);//is malloced
    return res;
}


