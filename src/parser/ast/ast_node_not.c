#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

static struct ast_node_not *create_ast_n_not_int(struct ast_node *child)
{
    struct ast_node_not *new = malloc(sizeof(struct ast_node_not));
    if (!new)
        return NULL;
    new->child = child;
    return new;
}

struct ast_node *create_ast_node_not(struct ast_node *child)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_not *under_node = create_ast_n_not_int(child);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_NOT;
    new->son = under_node;
    return new;
}

void destroy_ast_node_not(struct ast_node_not *node)
{
    destroy_ast(node->child);
    free(node);
}

void print_ast_not(struct ast_node_not *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"!\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->child, num, fd);
}
