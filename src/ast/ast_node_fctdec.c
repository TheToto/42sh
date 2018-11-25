/**
 * \file ast_node_fctdec.c
 * \author louis.holleville
 * \version 0.3
 * \date 14-11-2018
 * \brief Management of function declarations
 */

#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

static struct ast_node_fctdec *create_ast_node_fctdec_intern(char *name,
        struct ast_node *function)
{
    struct ast_node_fctdec *new = malloc(sizeof(struct ast_node_fctdec));
    if (!new)
        return NULL;
    new->name = name;
    new->function = function;
    return new;
}

struct ast_node *create_ast_node_fctdec(char *name,
        struct ast_node *function)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_fctdec *under_node = create_ast_node_fctdec_intern(name,
            function);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_FCTDEC;
    new->son = under_node;
    return new;
}

void destroy_ast_node_fctdec(struct ast_node_fctdec *node)
{
    if (!node)
        return;
    destroy_ast(node->function);
    free(node);
}

void print_ast_fctdec(struct ast_node_fctdec *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"Function %s\"];\n", *num, node->name);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->function, num, fd);
}
