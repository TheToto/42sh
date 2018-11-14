#include <stdlib.h>
#include "parser.h"
#include "ast.h"

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
    new->type = LOGICAL_OR;
    new->son->token_LOR = under_node;
    return new;
}
