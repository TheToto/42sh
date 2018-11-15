#include "ast.h"
#include "ast_destroy.h"

void destroy_ast(struct ast_node *root)
{
    switch (root->type)
    {
        case N_IF:
            ast_destroy_if(root->son);
            break;
        case N_FOR:
            ast_destroy_for(root->son);
            break;
        case N_WHILE:
            ast_destroy_while(root->son);
            break;
        case N_CASE:
            ast_destroy_case(root->son);
            break;
        case N_REDIRECT:
            ast_destroy_redirect(root->son);
            break;
        case N_SCMD:
            ast_destroy_scmd(root->son);
            break;
        case N_FCTDEC:
            ast_destroy_fctdec(root->son);
            break;
        case N_LOGICAL_AND:
            ast_destroy_land(root->son);
            break;
        case N_LOGICAL_OR:
            ast_destroy_lor(root->son);
            break;
        case N_AMPERSAND:
            ast_destroy_ampersand(root->son);
            break;
        case N_SEMICOLON:
            ast_destroy_semicolon(root->son);
            break;
        case N_NOT:
            ast_destroy_not(root->son);
            break;
        case N_PIPE:
            ast_destroy_pipe(root->son);
            break;
        default:
            break;
    }
}
