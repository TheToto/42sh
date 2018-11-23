/**
 *\file execution.h
 *\author sabrina.meng thomas.lupin
 *\version 0.5
 *\date 15-11-2018
 *\brief Header of AST exec
 */

#pragma once

#include "ast.h"
#include "var.h"

/**
 *\fn exec_main
 *\brief Send a string to lexer, parser, and exec
 *\param str  The string to execute
 *\return Return an int depending on the commands given
 */
int exec_main(char *str, int is_print, struct variables *library);

/**
 *\fn exec_node
 *\brief Execute the complete AST
 *\param struct ast_node *node  The AST node to execute
 *\return Return an int depending on the commands given
 */
int exec_node(struct ast_node *node, struct variables *var);

/**
 *\fn exec_not
 *\brief Execute the not node
 *\param struct ast_node_redirect *n_redirect  The AST node of the not
 */
int exec_not(struct ast_node_not *n_not, struct variables *var);

/**
 *\fn exec_redirect
 *\brief Execute the redirection
 *\param struct ast_node_redirect *n_redirect  The AST node of the redirection
 */
int exec_redirect(struct ast_node_redirect *n_redirect, struct variables *var);

/**
 *\fn exec_for
 *\brief Execute the for loop
 *\param struct ast_node_for *n_for   The AST node of the for loop
 */
int exec_for(struct ast_node_for *n_for, struct variables *var);

/**
 *\fn exec_semicolon
 *\brief Execute two child of semicolon ast
 *\param struct ast_node_semicolon *n_semi The AST node of the semilon separator
 */
int exec_semicolon(struct ast_node_semicolon *n_semi,
        struct variables *var);

/**
 *\fn exec_while
 *\brief Execute the while loop
 *\param struct ast_node_while *n_while   The AST node of the while loop
 */
int exec_while(struct ast_node_while *n_while, struct variables *var);

/**
 *\fn exec_if
 *\brief Execute the if command
 *\param struct ast_node_if *n_if   The AST node of the if command
 */
int exec_if(struct ast_node_if *n_if, struct variables *var);

/**
 *\fn exec_scmd
 *\brief Execute a simple command
 *\param struct ast_node_scmd *scmd The AST node of the simple command
 *\return Return an int depending on the command
 */
int exec_scmd(struct ast_node_scmd *scmd, struct variables *var);

/**
 *\fn exec_pipe
 *\brief Execute a pipe command
 *\param struct ast_node_pipe *pipe The AST node of the pipe command
 *\return Return an int depending on the command
 */
int exec_pipe(struct ast_node_pipe *n, struct variables *var);