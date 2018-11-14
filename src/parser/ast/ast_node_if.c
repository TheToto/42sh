#include "parser.h"
#include "ast.h"

static struct ast_node_if *create_ast_node_if_intern(struct ast_node *e_true,
        struct ast_node *e_false,
        struct ast_node *condition)
{
    struct ast_node_if *new = malloc(sizeof(struct ast_node_if));
    if (!new)
        return NULL;
    new->e_true = e_true;
    new->e_false = e_false;
    new->condition = condition;
    return new;
}

struct ast_node *create_ast_node_if(struct ast_node *e_true,
        struct ast_node *e_false, struct ast_node *condition)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node *under_node = create_ast_node_if_intern(e_true,
            e_false, condition);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = IF;
    new->son.token_IF = under_node;
    return new;
}
