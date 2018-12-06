/**
* \file ast_destroy.c
* \author louis.holleville
* \version 0.8
* \date 14-11-2018
* \brief General management of destroy files
*/

#include <stdlib.h>
#include "ast.h"
#include "ast_destroy.h"

void destroy_ast(struct ast_node *root)
{
    if (!root)
        return;
    switch (root->type)
    {
    case N_IF:
        destroy_ast_node_if(root->son);
        break;
    case N_FOR:
        destroy_ast_node_for(root->son);
        break;
    case N_WHILE:
        destroy_ast_node_while(root->son);
        break;
    case N_CASE:
        destroy_ast_node_case(root->son);
        break;
    case N_REDIRECT:
        destroy_ast_node_redirect(root->son);
        break;
    case N_SCMD:
        destroy_ast_node_scmd(root->son);
        break;
    case N_FCTDEC:
        destroy_ast_node_fctdec(root->son);
        break;
    case N_LOGICAL_AND:
        destroy_ast_node_land(root->son);
        break;
    case N_LOGICAL_OR:
        destroy_ast_node_lor(root->son);
        break;
    case N_AMPERSAND:
        destroy_ast_node_ampersand(root->son);
        break;
    case N_SEMICOLON:
        destroy_ast_node_semicolon(root->son);
        break;
    case N_NOT:
        destroy_ast_node_not(root->son);
        break;
    case N_PIPE:
        destroy_ast_node_pipe(root->son);
        break;
    default:
        break;
    }
    free(root);
}
