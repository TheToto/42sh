/**
* \file ast_node_if.c
* \author louis.holleville
* \version 1.0
* \date 14-11-2018
* \brief Management of if
*/

#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

static struct ast_node_if *create_ast_node_if_intern(struct ast_node *e_true,
        struct ast_node *e_false,
        struct ast_node *condition)
{
    struct ast_node_if *new = malloc(sizeof(struct ast_node_if));
    if (!new)
        return NULL;
    new->e_true = e_true;
    new->e_false = e_false;
    new->condition = condition;
    return new;
}

struct ast_node *create_ast_node_if(struct ast_node *e_true,
        struct ast_node *e_false, struct ast_node *condition)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_if *under_node = create_ast_node_if_intern(e_true,
            e_false, condition);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_IF;
    new->son = under_node;
    return new;
}

void destroy_ast_node_if(struct ast_node_if *node)
{
    destroy_ast(node->e_true);
    destroy_ast(node->e_false);
    destroy_ast(node->condition);
    free(node);
}

void print_ast_if(struct ast_node_if *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"IF\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu->%lu[label= \"true\"];\n", save, *num);
    print_ast_node(node->e_true, num, fd);

    *num += 1;
    fprintf(fd, "%lu->%lu[label= \"condition\"];\n", save, *num);
    print_ast_node(node->condition, num, fd);

    if (node->e_false)
    {
        *num += 1;
        fprintf(fd, "%lu->%lu[label= \"false\"];\n", save, *num);
        print_ast_node(node->e_false, num, fd);
    }
}
