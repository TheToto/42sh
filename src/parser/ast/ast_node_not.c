#include <stdlib.h>
#include "parser.h"
#include "ast.h"


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
    new->type = NOT;
    new->son->token_NOT = under_node;
    return new;
}
