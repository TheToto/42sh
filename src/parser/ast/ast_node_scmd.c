#include <stdlib.h>
#include <err.h>
#include "parser.h"
#include "ast.h"

static struct ast_node_scmd *create_ast_node_scmd_intern(void)
{
    struct ast_node_scmd *new = malloc(sizeof(struct ast_node_scmd));
    if (!new)
        return NULL;
    new->prefix = malloc(8 * sizeof(char*));
    if (!new->prefix)
    {
        free(new);
        return NULL;
    }
    new->elements = malloc(8 * sizeof(char*));
    if (!new->elements)
    {
        free(new->prefix);
        free(new);
    }
    new->elt_capacity = 8;
    new->pre_capacity = 8;
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
        char **new = realloc(cur->prefix, 2 * cur->pre_size);
        if (!new)
        {
            warnx("realloc failed in add_prefix_scmd(%s)", prefix);
            return;
        }
        cur->prefix = new;
        cur->pre_capacity *= 2;
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
        char **new = realloc(cur->elements, 2 * cur->elt_size);
        if (!new)
        {
            warnx("realloc failed in add_element_scmd(%s)", element);
            return;
        }
        cur->elements = new;
        cur->elt_capacity *= 2;
    }
    cur->elements[cur->elt_size] = element;
    cur->elt_size += 1;
}

