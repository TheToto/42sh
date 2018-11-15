#include <stdlib.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_for *create_ast_node_for_intern(char **values,
        char *value, struct ast_node *exec)
{
    struct ast_node_for *new = malloc(sizeof(struct ast_node_for));
    if (!new)
        return NULL;
    new->values = values;
    new->value = value;
    new->exec = exec;
    return new;
}

struct ast_node *create_ast_node_for(char **values,
        char *value, struct ast_node *exec)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_for *under_node = create_ast_node_for_intern(values,
            value, exec);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_FOR;
    new->son->token_FOR = under_node;
    return new;
}
