#include <stdlib.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_scmd *create_ast_node_scmd_intern(char **prefix,
        char **elements)
{
    struct ast_node_scmd *new = malloc(sizeof(struct ast_node_scmd));
    if (!new)
        return NULL;
    new->prefix = prefix;
    new->elements = elements;
    return new;
}

struct ast_node *create_ast_node_scmd(char **prefix, char **elements)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_scmd *under_node = create_ast_node_scmd_intern(prefix,
            elements);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_SCMD;
    new->son->token_SCMD = under_node;
    return new;
}
