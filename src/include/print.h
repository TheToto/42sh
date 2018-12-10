/**
* \file print.h
* \author louis.holleville
* \version 1.0
* \date 22-11-2018
* \brief Header of print_ast functions
*/

#pragma once

#include <stdio.h>

#include "ast.h"

/**
* General function to be called to print an AST
* @param ast root of the AST to be printed
* @param path path to the file where result will be written
*/
void makedot(struct ast_node *ast, char *path);

/**
* General function called recursively which redirect to the good ast_print func
* @param ast Node to be tested
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_node(struct ast_node *ast, size_t *num, FILE *f);

/**
* Specialized function to print 'ampersand' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_ampersand(struct ast_node_ampersand *node,
        size_t *num, FILE *fd);
/**
* Specialized function to print 'if' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_if(struct ast_node_if *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'for' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_for(struct ast_node_for *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'while' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_while(struct ast_node_while *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'case' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_case(struct ast_node_case *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'redirect' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_redirect(struct ast_node_redirect *node,
        size_t *num, FILE *fd);

/**
* Specialized function to print 'scmd' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_scmd(struct ast_node_scmd *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'function declaration' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_fctdec(struct ast_node_fctdec *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'logical or' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_lor(struct ast_node_lor *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'logical and' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_land(struct ast_node_land *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'semicolon' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_semicolon(struct ast_node_semicolon *node,
        size_t *num, FILE *fd);

/**
* Specialized function to print 'not' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_not(struct ast_node_not *node, size_t *num, FILE *fd);

/**
* Specialized function to print 'pipe' nodes
* @param num pointer to the number of the ast_node to be tested in the graph
* @param f opened file where info will be written
*/
void print_ast_pipe(struct ast_node_pipe *node, size_t *num, FILE *fd);
