#include "parser.h"
#include "ast.h"

//CODING STYLE DE MERDE
static struct ast_node_while *create_ast_node_while_int(struct ast_node *cond,
        struct ast_node *exec)
{
    struct ast_node_while *new = malloc(sizeof(struct ast_node_while));
    if (!new)
        return NULL;
    new->condition = cond;
    new->exec = exec;
    return new;
}

struct ast_node *create_ast_node_while(struct ast_node *condition, struct
        ast_node *exec)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node *under_node = create_ast_node_while_int(condition,
            exec);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = WHILE;
    new->son.token_WHILE = under_node;
    return new;
}
