/**
 *\file execution.h
 *\author thomas.lupin
 *\version 0.3
 *\date 15-11-2018
 *\brief Header of AST exec
 */

#pragma once

#include "ast.h"
#include "var.h"

int exec_main(char *str, int is_print, struct variables *library);
int launch_file(char *path, int is_print);

int exec_node(struct ast_node *node, struct variables *var);
