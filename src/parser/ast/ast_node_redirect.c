#include <stdlib.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_redirect *create_ast_node_redirect_intern(int fd,
        enum redirect_type type, int io_number, char *word)
{
    struct ast_node_redirect *new = malloc(sizeof(struct ast_node_redirect));
    if (!new)
        return NULL;
    new->fd = fd;
    new->io_number = io_number;
    new->word = word;
    new->type = type;
    return new;
}

struct ast_node *create_ast_node_redirect(int fd,
        enum redirect_type type, int io_number, char *word)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_redirect *under_node = create_ast_node_redirect_intern(fd,
            type, io_number, word);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = REDIRECT;
    new->son->token_REDIRECT = under_node;
    return new;
}
