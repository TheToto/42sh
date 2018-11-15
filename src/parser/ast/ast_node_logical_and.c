#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

static struct ast_node_land *create_ast_n_land_int(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node_land *new = malloc(sizeof(struct ast_node_land));
    if (!new)
        return NULL;
    new->left_child = left_child;
    new->right_child = right_child;
    return new;
}

struct ast_node *create_ast_node_land(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_land *under_node = create_ast_n_land_int(left_child,
            right_child);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_LOGICAL_AND;
    new->son = under_node;
    return new;
}

void destroy_ast_node_land(struct ast_node_land *node)
{
    destroy_ast(node->left_child);
    destroy_ast(node->right_child);
    free(node);
}

void print_ast_land(struct ast_node_land *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"&&\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->left_child, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->right_child, num, fd);
}
