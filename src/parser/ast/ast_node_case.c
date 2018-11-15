#include <stdlib.h>
#include <err.h>
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

static struct ast_node_case *create_ast_node_case_intern(char **values,
        struct ast_node *exec, struct ast_node_case *prev_case)
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
    new->values = arr;
    new->capacity = 8;
    new->size = 0;
    new->exec = exec;
    if (prev_case)
        prev_case->next = new;
    new->next = NULL;
    return new;
}

struct ast_node *create_ast_node_case(struct ast_node *exec,
        struct ast_node_case *prev_case)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (!new)
        return NULL;
    struct ast_node_case *under_node = create_ast_node_case_intern(exec,
            prev_case);

    if (!under_node)
    {
        free(new);
        return NULL;
    }
    new->type = N_CASE;
    new->son = under_node;
    return new;
}

void add_value_case(struct ast_node *node, char *value)
{
    if (!node || !value)
    {
        warnx("cannot do add_value_case: node or value is null");
        return;
    }
    if (node->type != token_CASE)
    {
        warnx("cannot do add_value_case: node is not case");
        return;
    }
    struct ast_node_case *cur = node->son;
    if (cur->size == cur->capacity)
    {
        char **new = realloc(cur->values, 2 * cur->capacity);
        if (!new)
        {
            warnx("cannot realloc in add_value_case");
            return;
        }
        cur->values = new;
        cur->capacity *= 2;
    }
    cur->values[cur->size] = value;
    cur->size += 1;
}

void destroy_ast_node_case(struct ast_node *node)
{
    struct ast_node_case *target = node->son;
    if (target->next)
        destroy_ast_node_case(target->next);
    destroy_ast(target->exec);
    free(target->values);
    free(node->son);
    free(node);
}

void print_ast_case(struct ast_node_case *node, size_t *num, FILE *fd)
{
    fprintf(fd, "%lu [label= \"CASE: ", *num);
    for (size_t i = 0; i < node->size; i++)
        fprintf(fd, "%s", node->values[i]);
    fprintf(fd, "\"];\n", *num);

    size_t save = *num;

    if (node->next)
    {
        *num += 1;
        fprintf(fd, "%lu -- %lu;\n", save, *num);
        print_ast_case(node->next, num, fd);
    }

    *num += 1;
    fprintf(fd, "%lu -> %lu;\n", save, *num);
    print_ast_node(node->exec, num, fd);
}
