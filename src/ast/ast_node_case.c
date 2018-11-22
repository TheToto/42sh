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
    new->value = value;
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
    if (!node || !value || !exec)
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
    cur->cases[cur->size] = value;
    cur->nodes[cur->size] = exec;
    cur->size += 1;
}

void destroy_ast_node_case(struct ast_node_case *node)
{
    for (size_t i = 0; i < node->size; i++)
        destroy_ast(node->nodes[i]);
    free(node->cases);
    free(node->nodes);
    free(node);
}

void print_ast_case(struct ast_node_case *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"CASE %s:\"]", *num, node->value);
    // Il faudra size liens avec la values[i] en label pointe vers le nodes[i]

    /*for (size_t i = 0; i < node->size; i++)
        fprintf(fd, "%s", node->values[i]);
    fprintf(fd, "\"];\n");

    size_t save = *num;

    if (node->next)
    {
        *num += 1;
        fprintf(fd, "%lu -- %lu;\n", save, *num);
        print_ast_case(node->next, num, fd);
    }

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->exec, num, fd);*/
}
