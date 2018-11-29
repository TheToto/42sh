/**
*\file execution.h
*\author sabrina.meng thomas.lupin louis.holleville
*\version 0.5
*\date 15-11-2018
*\brief Header of AST exec
*/

#pragma once

#include "ast.h"
#include "env.h"

/**
* Send a string to lexer, parser, and exec
* \param str  The string to execute
* \return Return an int depending on the commands given
*/
int exec_main(char *str, int is_print, struct variables *library);

/**
* Execute the complete AST
* \param struct ast_node *node  The AST node to execute
* \return Return an int depending on the commands given
*/
int exec_node(struct ast_node *node, struct variables *var);

/**
* Execute the not node
* \param struct ast_node_redirect *n_redirect  The AST node of the not
*/
int exec_not(struct ast_node_not *n_not, struct variables *var);

/**
* Execute the redirection
* \param struct ast_node_redirect *n_redirect  The AST node of the redirection
*/
int exec_redirect(struct ast_node_redirect *n_redirect, struct variables *var);

/**
* Execute the for loop
* param struct ast_node_for *n_for   The AST node of the for loop
*/
int exec_for(struct ast_node_for *n_for, struct variables *var);

/**
* Execute two child of semicolon ast
* \param struct ast_node_semicolon *n_semi The AST node of the semilon separator
*/
int exec_semicolon(struct ast_node_semicolon *n_semi,
        struct variables *var);

/**
* Execute the while loop
* \param struct ast_node_while *n_while   The AST node of the while loop
*/
int exec_while(struct ast_node_while *n_while, struct variables *var);

/**
* Execute the if command
* \param struct ast_node_if *n_if   The AST node of the if command
*/
int exec_if(struct ast_node_if *n_if, struct variables *var);

/**
* Execute a simple command
* \param struct ast_node_scmd *scmd The AST node of the simple command
* \return Return an int depending on the command
*/
int exec_scmd(struct ast_node_scmd *scmd, struct variables *var);

/**
* Execute a pipe command
* \param struct ast_node_pipe *pipe The AST node of the pipe command
* \return Return an int depending on the command
*/
int exec_pipe(struct ast_node_pipe *n, struct variables *var);


/**
* Execute a case statement
* \param struct ast_node_case *node The AST node of the case statement
* \return Return an int depending on the command
*/
int exec_case(struct ast_node_case *node, struct variables *var);

/**
* Execute a function declaration statement
* \param struct ast_node_case *node The AST node of the fctdec statement
* \param var the library of environnement variables
* \return Return an int depending on the command
*/
int exec_fctdec(struct ast_node_case *node, struct variables *var);

/**
* Execute a logical and
* \param struct ast_node_land *node The AST node of the logical and
* \param var the library of environnement variables
* \return Return an int depending on the command
*/
int exec_land(struct ast_node_land *node, struct variables *var);

/**
* Execute a logical or
* \param struct ast_node_lor *node The AST node of the logical or
* \param var the library of environnement variables
* \return Return an int depending on the command
*/
int exec_lor(struct ast_node_lor *node, struct variables *var);
