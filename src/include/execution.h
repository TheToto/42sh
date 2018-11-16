/**
 *\file execution.h
 *\author thomas.lupin
 *\version 0.3
 *\date 15-11-2018
 *\brief Header of AST exec
 */

#pragma once

#include "ast.h"

int exec_node(struct ast_node *node);
int exec_main(char *str);
void launch_file(char *path);
