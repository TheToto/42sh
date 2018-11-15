#pragma once

#include <stdio.h>

void makedot(struct ast_node *ast, char *path);
void print_ast_node(struct ast_node *ast, size_t *num,FILE *f);
