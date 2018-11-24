/**
 *\file execution.c
 *\author sabrina.meng thomas.lupin
 *\version 0.3
 *\date 15-11-2018
 *\brief Execution of the AST
 */
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "var.h"
#include "options.h"
#include "lexer.h"
#include "print.h"
#include "execution.h"
#include "parser.h"
#include "ast_destroy.h"

int exec_scmd(struct ast_node_scmd *scmd, struct variables *var)
{
    //if !builtin cmd
    pid_t pid;
    int status;
    int error = 0;
    for (size_t i = 0; i < scmd->pre_size; i++)
        assign_prefix(var, scmd->prefix[i]);
    char **expanded = replace_var_scmd(var, scmd);
    if (scmd->elt_size > 0)
    {
        pid = fork();
        if (pid < 0)
            errx(1, "ERROR: Fork failed");
        else if (pid == 0)
        {
            error = execvp(*expanded, expanded);
            if (error < 0)
                err(127, "Exec %s failed", *expanded);
        }
        else
        {
            while (waitpid(pid, &status, 0) != pid)
                continue;
        }
        status = WEXITSTATUS(status);
        printf("%s return %d\n", *scmd->elements, status);
    }
    for (size_t i = 0; i < scmd->elt_size + 1; i++)
        free(expanded[i]);
    free(expanded);
    return status;
}

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
    while (exec_node(n_while->condition, var) == 0)
        res = exec_node(n_while->exec, var);
    return res;
}

int exec_for(struct ast_node_for *n_for, struct variables *var)
{
    char *name = n_for->value;
    int res = 0;
    for (size_t i = 0; i < n_for->size; i++)
    {
        add_var(var, name, n_for->values[i]);
        res = exec_node(n_for->exec, var);
    }
    return res;
}

int exec_semicolon(struct ast_node_semicolon *n_semi,
        struct variables *var)
{
    exec_node(n_semi->left_child, var);
    return exec_node(n_semi->right_child, var);
}

int exec_not(struct ast_node_not *n_not, struct variables *var)
{
    return exec_node(n_not->child, var) == 0;
}

int exec_node(struct ast_node *node, struct variables *var)
{
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
        case N_NONE:
            return 0;
        default:
            break;
    }
    return 0;
}

int exec_main(char *str, int is_print, struct variables *library)
{
    //printf("exec: %s\n", str);
    struct lexer *l = lexer(str);
    struct token_list *copy = l->token_list;
    struct ast_node *ast = rule_input(&(l->token_list));
    l->token_list = copy;

    if (!ast)
    {
        lexer_destroy(l);
        warnx("Error in parsing");
        return 1;
    }

    if (is_print)
        makedot(ast, "ast.dot");

    printf("\nExecution result:\n");
    int res = exec_node(ast, library);

    destroy_ast(ast);
    lexer_destroy(l);

    return res;
}
