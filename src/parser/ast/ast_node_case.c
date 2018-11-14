#include <stdlib.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_case *create_ast_node_case_intern(char **values,
        struct ast_node *exec, struct ast_node_case *prev_case)
{
    struct ast_node_case *new = malloc(sizeof(struct ast_node_case));
    if (!new)
        return NULL;
    new->values = values;
    new->exec = exec;
    if (prev_case)
        prev_case->next = new;
    new->next = NULL;
    return new;
}

struct ast_node *create_ast_node_case(char **values,
        struct ast_node *exec, struct ast_node_case *prev_case)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_case *under_node = create_ast_node_case_intern(values,
        exec, prev_case);

    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_CASE;
    new->son->token_CASE = under_node;
    return new;
}
