#include <stdlib.h>
#include <err.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

static struct ast_node_scmd *create_ast_node_scmd_intern(void)
{
    struct ast_node_scmd *new = malloc(sizeof(struct ast_node_scmd));
    if (!new)
        return NULL;
    new->prefix = calloc(8, sizeof(char*));
    if (!new->prefix)
    {
        free(new);
        return NULL;
    }
    new->elements = calloc(8, sizeof(char*));
    if (!new->elements)
    {
        free(new->prefix);
        free(new);
    }
    new->elt_capacity = 7;
    new->pre_capacity = 7;
    new->elt_size = 0;
    new->pre_size = 0;
    return new;
}

struct ast_node *create_ast_node_scmd(void)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_scmd *under_node = create_ast_node_scmd_intern();
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_SCMD;
    new->son = under_node;
    return new;
}

void add_prefix_scmd(struct ast_node *node, char *prefix)
{
    if (!node || !prefix)
    {
        warnx("could not add prefix in simple command node: node or prefix\
                is empty");
        return;
    }
    if (node->type != N_SCMD)
    {
        warnx("could not add prefix in simple command node: node is not\
                simple command node");
        return;

    }
    struct ast_node_scmd *cur = node->son;
    if (cur->pre_size == cur->pre_capacity)
    {
        cur->pre_capacity += 1;
        char **new = realloc(cur->prefix,
                2 * cur->pre_capacity * sizeof(char*));
        if (!new)
        {
            warnx("realloc failed in add_prefix_scmd(%s)", prefix);
            return;
        }
        cur->prefix = new;
        cur->pre_capacity *= 2;
        for (size_t i = cur->pre_size; i < cur->pre_capacity; i++)
            cur->prefix[i] = NULL;
        cur->pre_capacity -= 1;
    }
    cur->prefix[cur->pre_size] = prefix;
    cur->pre_size += 1;
}

void add_element_scmd(struct ast_node *node, char *element)
{
    if (!node || !element)
    {
        warnx("could not add element in simple command node: node or element\
                is empty");
        return;
    }
    if (node->type != N_SCMD)
    {
        warnx("could not add element in simple command node: node is not\
                simple command node");
        return;

    }
    struct ast_node_scmd *cur = node->son;
    if (cur->elt_size == cur->elt_capacity)
    {
        cur->elt_capacity += 1;
        char **new = realloc(cur->elements,
                2 * cur->elt_capacity * sizeof(char*));
        if (!new)
        {
            warnx("realloc failed in add_element_scmd(%s)", element);
            return;
        }
        cur->elements = new;
        cur->elt_capacity *= 2;
        for (size_t i = cur->elt_size; i < cur->elt_capacity; i++)
            cur->elements[i] = NULL;
        cur->elt_capacity -= 1;
    }
    cur->elements[cur->elt_size] = element;
    cur->elt_size += 1;
}

void destroy_ast_node_scmd(struct ast_node_scmd *node)
{
    free(node->elements);
    free(node->prefix);
    free(node);
}

void print_ast_scmd(struct ast_node_scmd *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"", *num);
    for (size_t i = 0; i < node->pre_size; i++)
        fprintf(fd, "%s ", node->prefix[i]);

    for (size_t i = 0; i < node->elt_size; i++)
        fprintf(fd, "%s ", node->elements[i]);

    fprintf(fd, "\"];\n");
}
