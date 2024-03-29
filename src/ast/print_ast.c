/**
* \file print_ast.c
* \author louis.holleville
* \version 1.0
* \date 14-11-2018
* \brief General management of ast-print
*/

#include <err.h>
#include "ast.h"
#include "print.h"

static void print_ast_bis(struct ast_node *ast, size_t *num, FILE *f)
{
    switch (ast->type)
    {
    case N_FCTDEC:
        print_ast_fctdec(ast->son, num, f);
        return;
    case N_LOGICAL_AND:
        print_ast_land(ast->son, num, f);
        return;
    case N_LOGICAL_OR:
        print_ast_lor(ast->son, num, f);
        return;
    case N_AMPERSAND:
        print_ast_ampersand(ast->son, num, f);
        return;
    case N_SEMICOLON:
        print_ast_semicolon(ast->son, num, f);
        return;
    case N_NOT:
        print_ast_not(ast->son, num, f);
        return;
    case N_PIPE:
        print_ast_pipe(ast->son, num, f);
        return;
    case N_NONE:
        return;
    default:
        return;
    }
}

void print_ast_node(struct ast_node *ast, size_t *num, FILE *f)
{
    if (!ast)
        return;
    switch (ast->type)
    {
    case N_IF:
        print_ast_if(ast->son, num, f);
        return;
    case N_FOR:
        print_ast_for(ast->son, num, f);
        return;
    case N_WHILE:
        print_ast_while(ast->son, num, f);
        return;
    case N_CASE:
        print_ast_case(ast->son, num, f);
        return;
    case N_REDIRECT:
        print_ast_redirect(ast->son, num, f);
        return;
    case N_SCMD:
        print_ast_scmd(ast->son, num, f);
        return;
    default:
        print_ast_bis(ast, num, f);
    }
}

void makedot(struct ast_node *ast, char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
        err(1, "Can't open %s", path);
    fprintf(f, "digraph AST {\n");
    size_t i = 0;
    print_ast_node(ast, &i, f);
    fprintf(f, "}\n");
    fclose(f);
}
