#pragma once

#include "ast.h"
#include "var.h"

/**
 *\fn exec_node
 *\brief Execute the complete AST
 *\param struct ast_node *node  The AST node to execute
 *\return Return an int depending on the commands given
 */
int exec_node(struct ast_node *node, struct variables *var);
