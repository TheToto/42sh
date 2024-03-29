/**
* @file exec_fctdec.c
* @author louis.holleville
* @version 1.0
* @date 25-11-2018
* @brief execution of functions declaration
*/

#include "ast.h"
#include "env.h"

int exec_fctdec(struct ast_node_fctdec *f, struct variables *var)
{
    if (!f || !var)
        return 1;
    if (add_func(var, f->name, f->function, DECLARED))
        return 1;
    f->function = NULL;
    return 0;
}
