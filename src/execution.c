#include <unistd.h>

#include "execution.h"

int exec_node(struct ast_node *node);

static int exec_scmd(struct ast_node *node)
{
    struct ast_node_scmd *scmd = node->son->token_SCMD;
    char *cmd = scmd->elements[0];
    char **args = scmd->elements + 1;
    return execvp(cmd, args);
}

static void exec_if(struct ast_node *node)
{
    struct ast_node_if *n_if = node->son->token_IF;
    int res = exec_node(n_if->condition);
    if (res)
        exec_node(n_if->e_true);
    else
        exec_node(n_if->e_false);
}

static void exec_while(struct ast_node *node)
{
    struct ast_node_while *n_while = node->son->token_WHILE;
    while (exec_node(n_while->condition))
        exec_node(n_while->exec);
}

static void exec_for(struct ast_node *node)
{
    struct ast_node_for *n_for = node->son->token_FOR;
    n_for = n_for;
}

int exec_node(struct ast_node *node)
{
    switch (node->type)
    {
        case N_SCMD:
            return exec_scmd(node);
        case N_IF:
            exec_if(node);
            break;
        case N_WHILE:
            exec_while(node);
            break;
        case N_FOR:
            exec_for(node);
            break;
        default:
            break;
    }
    return 0;
}
