/**
 *\file print.h
 *\author louis.holleville
 *\version 0.5
 *\date 22-11-2018
 *\brief Header of print_ast functions
 */

#pragma once

#include <stdio.h>

#include "ast.h"

void makedot(struct ast_node *ast, char *path);
void print_ast_node(struct ast_node *ast, size_t *num,FILE *f);

void print_ast_ampersand(struct ast_node_ampersand *node,
        size_t *num, FILE *fd);
void print_ast_if(struct ast_node_if *node, size_t *num, FILE *fd);
void print_ast_for(struct ast_node_for *node, size_t *num, FILE *fd);
void print_ast_while(struct ast_node_while *node, size_t *num, FILE *fd);
void print_ast_case(struct ast_node_case *node, size_t *num, FILE *fd);
void print_ast_redirect(struct ast_node_redirect *node,
        size_t *num, FILE *fd);
void print_ast_scmd(struct ast_node_scmd *node, size_t *num, FILE *fd);
void print_ast_fctdec(struct ast_node_fctdec *node, size_t *num, FILE *fd);
void print_ast_lor(struct ast_node_lor *node, size_t *num, FILE *fd);
void print_ast_land(struct ast_node_land *node, size_t *num, FILE *fd);
void print_ast_semicolon(struct ast_node_semicolon *node,
        size_t *num, FILE *fd);
void print_ast_not(struct ast_node_not *node, size_t *num, FILE *fd);
void print_ast_pipe(struct ast_node_pipe *node, size_t *num, FILE *fd);
