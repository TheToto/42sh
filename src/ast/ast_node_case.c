/**
* \file ast_node_case.c
* \author louis.holleville
* \version 0.9
* \date 14-11-2018
* \brief Management of case
*/

#define _DEFAULT_SOURCE
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "parser.h"
#include "ast.h"
#include "print.h"
#include "ast_destroy.h"

static struct ast_node_case *create_ast_node_case_intern(char *value)
{
    struct ast_node_case *new = malloc(sizeof(struct ast_node_case));
    if (!new)
        return NULL;
    char **arr = malloc(8 * sizeof(char*));
    if (!arr)
    {
        free(new);
        warnx("cannot malloc array in create_ast_node_case");
        return NULL;
    }
    struct ast_node **asts = malloc(8 * sizeof(struct ast_node*));
    if (!asts)
    {
        free(new);
        free(arr);
        warnx("cannot malloc array in create_ast_node_case");
        return NULL;
    }
    new->cases = arr;
    new->nodes = asts;
    new->capacity = 8;
    new->size = 0;
    new->value = strdup(value);
    return new;
}

struct ast_node *create_ast_node_case(char *value)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_case *under_node = create_ast_node_case_intern(value);

    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_CASE;
    new->son = under_node;
    return new;
}

void add_case_value(struct ast_node *node, char *value, struct ast_node *exec)
{
    if (!node || !value)
    {
        warnx("cannot do add_value_case: exec or node or value is null");
        return;
    }
    struct ast_node_case *cur = node->son;
    if (cur->size == cur->capacity)
    {
        char **new = realloc(cur->cases, 2 * cur->capacity * sizeof(char*));
        if (!new)
        {
            warnx("cannot realloc in add_value_case");
            return;
        }
        cur->cases = new;
        struct ast_node **new2 = realloc(cur->nodes, 2 * cur->capacity
                * sizeof(struct ast_node*));
        if (!new)
        {
            warnx("cannot realloc in add_value_case");
            return;
        }
        cur->nodes = new2;
        cur->capacity *= 2;

    }
    cur->cases[cur->size] = strdup(value);
    cur->nodes[cur->size] = exec;
    cur->size += 1;
}

static void destroy_case_null(struct ast_node_case *node,
        struct ast_node *to_null)
{
    for (size_t i = 0; i < node->size; i++)
    {
        if (node->nodes[i] == to_null)
        {
            node->nodes[i] = NULL;
        }
    }
}

void destroy_ast_node_case(struct ast_node_case *node)
{
    for (size_t i = 0; i < node->size; i++)
    {
        free(node->cases[i]);
        destroy_ast(node->nodes[i]);
        destroy_case_null(node, node->nodes[i]);
    }
    free(node->cases);
    free(node->nodes);
    free(node->value);
    free(node);
}

static char *put_back(char *str)
{
    char *res = calloc(strlen(str) + 100, sizeof(char));
    size_t j = 0;
    for (size_t i = 0; str[i]; i++, j++)
    {
        if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
        {
            res[j++] = '\\';
        }
        res[j] = str[i];
    }
    return res;
}

void print_ast_case(struct ast_node_case *node, size_t *num, FILE *fd)
{
    char *mal = put_back(node->value);
    fprintf(fd, "%lu [label= \"CASE %s:\"]\n", *num, mal);
    free(mal);

    size_t save = *num;

    for (size_t i = 0; i < node->size; i++)
    {
        *num += 1;
        char *mal = put_back(node->cases[i]);
        fprintf(fd, "%lu->%lu[label= \"%s\"];\n", save, *num, mal);
        free(mal);
        print_ast_node(node->nodes[i], num, fd);
    }
}
