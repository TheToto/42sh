#include <stdlib.h>
#include "parser.h"
#include "ast.h"

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
    new->son->token_SEMICOLON = under_node;
    return new;
}
