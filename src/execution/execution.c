/**
*\file execution.c
*\author sabrina.meng thomas.lupin
*\version 0.9
*\date 15-11-2018
*\brief Execution of the AST
*/
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "env.h"
#include "options.h"
#include "lexer.h"
#include "quote_lexer.h"
#include "print.h"
#include "execution.h"
#include "parser.h"
#include "ast_destroy.h"
#include "shell.h"
#include "queue.h"

int exec_if(struct ast_node_if *n_if, struct variables *var)
{
    int res = exec_node(n_if->condition, var);
    if (res == 0)
        return exec_node(n_if->e_true, var);
    else
        return exec_node(n_if->e_false, var);
}

int exec_while(struct ast_node_while *n_while, struct variables *var)
{
    int res = 0;
    shell.loop += 1;
    while (exec_node(n_while->condition, var) == 0)
    {
        res = exec_node(n_while->exec, var);
        if (shell.n_continue == -1 || shell.n_break == -1)
            break;
        if (shell.n_continue > 1 && shell.loop > 1)
        {
            shell.n_continue -= 1;
            break;
        }
        else if (shell.n_continue == 1)
            shell.n_continue -= 1;
        else if (shell.n_break)
        {
            shell.n_break -= 1;
            if (shell.loop == 1)
                shell.n_break = 0;
            break;
        }
    }
    shell.loop -= 1;
    return res;
}

int exec_for(struct ast_node_for *n_for, struct variables *var)
{
    char *name = n_for->value;
    int res = 0;
    shell.loop += 1;
    char **array = replace_var_for(n_for);
    for (size_t i = 0; array[i]; i++)
    {
        add_var(var, name, array[i], 0);
        res = exec_node(n_for->exec, var);
        if (shell.n_continue == -1 || shell.n_break == -1)
            break;
        if (shell.n_continue > 1 && shell.loop > 1)
        {
            shell.n_continue -= 1;
            break;
        }
        else if (shell.n_continue == 1)
            shell.n_continue -= 1;
        else if (shell.n_break)
        {
            shell.n_break -= 1;
            if (shell.loop == 1)
                shell.n_break = 0;
            break;
        }
    }
    shell.loop -= 1;
    for (size_t i = 0; array[i]; i++)
        free(array[i]);
    free(array);
    return res;
}

int exec_semicolon(struct ast_node_semicolon *n_semi,
        struct variables *var)
{
    exec_node(n_semi->left_child, var);
    if (shell.n_continue || shell.n_break)
        return 0;
    return exec_node(n_semi->right_child, var);
}

int exec_not(struct ast_node_not *n_not, struct variables *var)
{
    return exec_node(n_not->child, var) == 0;
}

int exec_node(struct ast_node *node, struct variables *var)
{
    if (!node)
        return 0; // For ampersand
    switch (node->type)
    {
    case N_SCMD:
        return exec_scmd(node->son, var);
    case N_IF:
        return exec_if(node->son, var);
    case N_WHILE:
        return exec_while(node->son, var);
    case N_FOR:
        return exec_for(node->son, var);
    case N_REDIRECT:
        return exec_redirect(node->son, var);
    case N_SEMICOLON:
        return exec_semicolon(node->son, var);
    case N_AMPERSAND:
        return exec_semicolon(node->son, var);
    case N_NOT:
        return exec_not(node->son, var);
    case N_PIPE:
        return exec_pipe(node->son, var);
    case N_CASE:
        return exec_case(node->son, var);
    case N_FCTDEC:
        return exec_fctdec(node->son, var);
    case N_LOGICAL_OR:
        return exec_lor(node->son, var);
    case N_LOGICAL_AND:
        return exec_land(node->son, var);
    case N_NONE:
        return 0;
    default:
        break;
    }
    return 0;
}

int exec_main(char *str, int is_print, struct variables *library)
{
    struct lexer *l = lexer(str);
    struct token_list *copy = l->token_list;
    struct ast_node *ast = rule_input(&copy);
    shell.ast = ast;
    lexer_destroy(shell.lexer);
    if (!ast)
    {
        warnx("Error in parsing");
        if (shell.type == S_OPTION)
            return 1;
        return 2;
    }

    if (is_print)
        makedot(ast, "ast.dot");

    int res = exec_node(ast, library);

    shell.ast = NULL;
    destroy_ast(ast);

    return res;
}
