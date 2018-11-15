#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"

static struct ast_node_pipe *create_ast_node_pipe_int(struct ast_node *ls,
        struct ast_node *rs)
{
    struct ast_node_pipe *new = malloc(sizeof(struct ast_node_pipe));
    if (!new)
        return NULL;
    new->ls = ls;
    new->rs= rs;
    return new;
}

struct ast_node *create_ast_node_pipe(struct ast_node *ls,
        struct ast_node *rs)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_pipe *under_node = create_ast_node_pipe_int(ls, rs);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_PIPE;
    new->son = under_node;
    return new;
}

void print_ast_pipe(struct ast_node_pipe *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"|\"];\n", *num);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->ls, num, fd);

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->rs, num, fd);
}

