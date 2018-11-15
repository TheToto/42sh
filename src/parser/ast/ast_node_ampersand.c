#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"

static struct ast_node_ampersand *c_a_n_amp_int(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node_ampersand *new = malloc(sizeof(struct ast_node_ampersand));
    if (!new)
        return NULL;
    new->left_child = left_child;
    new->right_child = right_child;
    return new;
}

struct ast_node *create_ast_node_ampersand(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_ampersand *under_node = c_a_n_amp_int(left_child,
            right_child);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_AMPERSAND;
    new->son = under_node;
    return new;
}


void print_ast_ampersand(struct ast_node_ampersand *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label=\"&\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->left_child, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->right_child, num, fd);
}

