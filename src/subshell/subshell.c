/**
* @file subshell.c
* @author thomas.lupin louis.holleville
* @date 07-12-2018
* @version 0.9
* Management of subshells
*/

#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "subshell.h"

int core_exec_subshell(struct ast_node *sub_root, struct variables *var,
        char *output)
{
    int fd = open("/tmp", O_TMPFILE | O_RDWR);
    int save = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    int res = exec_node(sub_root, var);

    dup2(save, STDOUT_FILENO);
    output = fd_to_str(fd);//is malloced
    return res;
}
