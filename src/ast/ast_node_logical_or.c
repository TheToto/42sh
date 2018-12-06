/**
* \file ast_node_logical_or.c
* \author louis.holleville
* \version 0.8
* \date 14-11-2018
* \brief Management of logical or
*/

#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

static struct ast_node_lor *create_ast_n_lor_int(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node_lor *new = malloc(sizeof(struct ast_node_lor));
    if (!new)
        return NULL;
    new->left_child = left_child;
    new->right_child = right_child;
    return new;
}

struct ast_node *create_ast_node_lor(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_lor *under_node = create_ast_n_lor_int(left_child,
            right_child);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_LOGICAL_OR;
    new->son = under_node;
    return new;
}

void destroy_ast_node_lor(struct ast_node_lor *node)
{
    destroy_ast(node->left_child);
    destroy_ast(node->right_child);
    free(node);
}

void print_ast_lor(struct ast_node_lor *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"||\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->left_child, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->right_child, num, fd);
}
