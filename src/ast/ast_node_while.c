/**
* \file ast_node_while.c
* \author louis.holleville
* \version 0.8
* \date 14-11-2018
* \brief Management of whiles
*/

#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

//CODING STYLE DE MERDE
static struct ast_node_while *create_ast_node_while_int(struct ast_node *cond,
        struct ast_node *exec)
{
    struct ast_node_while *new = malloc(sizeof(struct ast_node_while));
    if (!new)
        return NULL;
    new->condition = cond;
    new->exec = exec;
    return new;
}

struct ast_node *create_ast_node_while(struct ast_node *condition,
        struct ast_node *exec)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_while *under_node = create_ast_node_while_int(condition,
            exec);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_WHILE;
    new->son = under_node;
    return new;
}

void destroy_ast_node_while(struct ast_node_while *node)
{
    destroy_ast(node->exec);
    destroy_ast(node->condition);
    free(node);
}

void print_ast_while(struct ast_node_while *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"WHILE\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu[label= \"condition\"];\n", save, *num);
    print_ast_node(node->condition, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu[label= \"do\"];\n", save, *num);
    print_ast_node(node->exec, num, fd);
}
