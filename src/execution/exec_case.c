/**
*\file exec_case.c
*\author thomas.lupin
*\version 0.8
*\date 23-11-2018
*\brief Execution of the case node
*/
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fnmatch.h>

#include "env.h"
#include "options.h"
#include "lexer.h"
#include "print.h"
#include "execution.h"
#include "parser.h"
#include "ast_destroy.h"
#include "queue.h"

int exec_case(struct ast_node_case *node, struct variables *var)
{
    char *str = concat_quote(node->value);
    for (size_t i = 0; i < node->size; i++)
    {
        if (!fnmatch(node->cases[i], str, 0))
        {
            free(str);
            return exec_node(node->nodes[i], var);
        }
    }
    free(str);
    return 0;
}
