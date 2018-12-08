/**
* \file ast_node_for.c
* \author louis.holleville
* \version 0.9
* \date 14-11-2018
* \brief Management of for
*/

#define _DEFAULT_SOURCE
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

static struct ast_node_for *create_ast_node_for_intern(char *value,
        struct ast_node *exec)
{
    struct ast_node_for *new = malloc(sizeof(struct ast_node_for));
    if (!new)
    {
        warnx("cannot malloc in ast_node_for_intern");
        return NULL;
    }
    char **arr= calloc(8, sizeof(char*));
    if (!arr)
    {
        free(new);
        warnx("cannot malloc in ast_node_for_intern");
        return NULL;
    }
    new->values = arr;
    new->size = 0;
    new->capacity = 8;
    new->value = strdup(value);
    new->exec = exec;
    return new;
}

struct ast_node *create_ast_node_for(char *value, struct ast_node *exec)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_for *under_node = create_ast_node_for_intern(value, exec);
    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_FOR;
    new->son = under_node;
    return new;
}

void add_value_for(struct ast_node *node, char *value)
{
    if (!node || !value)
    {
        warnx("cannot do add_value_for: node or value is null");
        return;
    }
    if (node->type != N_FOR)
    {
        warnx("cannot do add_value_for: node is not case");
        return;
    }
    struct ast_node_for *cur = node->son;
    if (cur->size == cur->capacity - 1)
    {
        char **new = realloc(cur->values, 2 * cur->capacity * sizeof(char*));
        if (!new)
        {
            warnx("cannot realloc in add_value_for");
            return;
        }
        cur->values = new;
        cur->capacity *= 2;
    }
    cur->values[cur->size] = strdup(value);
    cur->values[cur->size + 1] = NULL;
    cur->size += 1;
}

void destroy_ast_node_for(struct ast_node_for *node)
{
    free(node->value);
    for (size_t i = 0; node->values[i]; i++)
        free(node->values[i]);
    destroy_ast(node->exec);
    free(node->values);
    free(node);
}

void print_ast_for(struct ast_node_for *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"FOR %s IN ", *num, node->value);
    size_t save = *num;
    for (size_t i = 0; i < node->size - 1; i++)
        fprintf(fd, "%s, ", node->values[i]);
    fprintf(fd, "%s", node->values[node->size - 1]);
    fprintf(fd, "\"];\n");

    *num += 1;
    fprintf(fd, "%lu->%lu[label= \"do\"];\n", save, *num);
    print_ast_node(node->exec, num, fd);
}
