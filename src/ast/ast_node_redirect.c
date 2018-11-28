/**
 * \file ast_node_redirect.c
 * \author louis.holleville
 * \version 0.3
 * \date 14-11-2018
 * \brief Management of redirections
 */

#define _DEFAULT_SOURCE
#include <string.h>
#include <stdlib.h>
#include <err.h>
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
        err(1, "cannot malloc in create_ast_node_redirect_intern (new)");
    char **words = calloc(8, sizeof(char*));
    if (!words)
        err(1, "cannot calloc in create_ast_node_redirect_intern (words)");
    new->fd = fd;
    new->io_number = io_number;
    new->word = strdup(word);
    new->words = words;
    new->size = 0;
    new->capacity = 8;
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

void add_elt_heredoc(struct ast_node *node, char *word)
{
    if (!node || !word)
    {
        warnx("cannot do add_elt_redirect: node or value is null");
        return;
    }
    if (node->type != N_REDIRECT)
    {
        warnx("cannot do add_elt_redirect: node is not case");
        return;
    }
    struct ast_node_redirect *cur = node->son;
    if (cur->size == cur->capacity)
    {
        char **new = realloc(cur->words, 2 * cur->capacity * sizeof(char*));
        if (!new)
        {
            warnx("cannot realloc in add_elt_redirect");
            return;
        }
        cur->words = new;
        cur->capacity *= 2;
    }
    cur->words[cur->size] = strdup(word);
    cur->size += 1;
}

void destroy_ast_node_redirect(struct ast_node_redirect *node)
{
    for (size_t i = 0; i < node->size; i++)
        free(node->words[i]);
    free(node->words);
    free(node->word);
    destroy_ast(node->node);
    free(node);
}

void print_ast_redirect(struct ast_node_redirect *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"REDIR: %d (%d) %s\"];\n", *num, node->io_number,
            node->type, node->word);
    size_t save = *num;
    *num += 1;

    for (size_t i = 0; i < node->size; i++)
    {
        fprintf(fd, "%lu -> %lu;\n%lu [label = \"%s\"];\n", save, *num, *num,
                node->words[i]);
        *num += 1;
    }
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->node, num, fd);
}
