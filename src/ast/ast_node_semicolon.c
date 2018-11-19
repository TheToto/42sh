#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

static struct ast_node_semicolon *c_a_n_sem_int(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node_semicolon *new = malloc(sizeof(struct ast_node_semicolon));
    if (!new)
        return NULL;
    new->left_child = left_child;
    new->right_child = right_child;
    return new;
}

struct ast_node *create_ast_node_semicolon(struct ast_node *left_child,
        struct ast_node *right_child)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_semicolon *under_node = c_a_n_sem_int(left_child,
            right_child);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_SEMICOLON;
    new->son = under_node;
    return new;
}

void destroy_ast_node_semicolon(struct ast_node_semicolon *node)
{
    destroy_ast(node->left_child);
    destroy_ast(node->right_child);
    free(node);
}

void print_ast_semicolon(struct ast_node_semicolon *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \";\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->left_child, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->right_child, num, fd);
}

