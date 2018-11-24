#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

static struct ast_node_redirect *create_ast_node_redirect_intern(int fd,
        enum redirect_type type, int io_number, char *word,
        struct ast_node *node)
{
    struct ast_node_redirect *new = malloc(sizeof(struct ast_node_redirect));
    if (!new)
        return NULL;
    new->fd = fd;
    new->io_number = io_number;
    new->word = word;
    new->type = type;
    new->node = node;
    return new;
}

struct ast_node *create_ast_node_redirect(int fd, enum redirect_type type,
        int io_number, char *word, struct ast_node *node)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_redirect *under_node = create_ast_node_redirect_intern(fd,
            type, io_number, word, node);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_REDIRECT;
    new->son = under_node;
    return new;
}

void destroy_ast_node_redirect(struct ast_node_redirect *node)
{
    destroy_ast(node->node);
    free(node);
}

void print_ast_redirect(struct ast_node_redirect *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"REDIR: %d (%d) %s\"];\n", *num, node->io_number,
            node->type, node->word);
    size_t save = *num;

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->node, num, fd);
}
