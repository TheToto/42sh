/**
 * @file exec_logical.c
 * @author thomas.lupin
 * @version 0.5
 * @date 25-11-2018
 * execution of logical and + logical or
 */

#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "execution.h"
#include "parser.h"
#include "ast_destroy.h"

int exec_land(struct ast_node_land *node, struct variables *var)
{
    int res = exec_node(node->left_child, var);
    if (res == 0)
        res = exec_node(node->right_child, var);
    return res;
}

int exec_lor(struct ast_node_lor *node, struct variables *var)
{
    int res = exec_node(node->left_child, var);
    if (res != 0)
        res = exec_node(node->right_child, var);
    return res;
}
