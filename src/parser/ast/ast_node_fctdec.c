#include <stdlib.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_fctdec *create_ast_node_fctdec_intern(char *name,
        struct ast_node *function)
{
    struct ast_node_fctdec *new = malloc(sizeof(struct ast_node_fctdec));
    if (!new)
        return NULL;
    new->name = name;
    new->function = function;
    return new;
}

struct ast_node *create_ast_node_fctdec(char *name,
        struct ast_node *function)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_fctdec *under_node = create_ast_node_fctdec_intern(name,
            function);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_FCTDEC;
    new->son->token_FCTDEC = under_node;
    return new;
}
