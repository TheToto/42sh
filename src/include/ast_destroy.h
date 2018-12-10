/**
* @file ast_destroy.h
* @author louis.holleville
* @version 1.0
* @date 14-11-2018
* @brief Header of AST destroy functions
*/
#include "ast.h"

/**
* Destroying function for 'simple_command' nodes. Recursively called.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_scmd(struct ast_node_scmd *node);

/**
* Destroying function for 'ampersand' node.
* @details Recursively calls destroy_ast on its \a left_child and \a
* right_child.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_ampersand(struct ast_node_ampersand *node);

/**
* Destroying function for 'if' node.
* @details Recursively calls destroy_ast ont its elements \a e_true \a e_false
* and \a condition.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_if(struct ast_node_if *node);

/**
* Destroying function for 'pipe' node.
* @details Recursively calls destroy_ast ont its elements \a ls and \a rs.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_pipe(struct ast_node_pipe *node);

/**
* Destroying function for 'while' node.
* @details Recursively calls destroy_ast ont its elements \a exec and
* \a condition.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_while(struct ast_node_while *node);

/**
* Destroying function for 'case' node.
* @details Recursively calls destroy_ast ont its element \a exec and
* recursively call itself on all cases of the switch.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_case(struct ast_node_case *node);

/**
* Destroying function for '&&' node (logical and)
* @details Recursively calls destroy_ast ont its element \a left_child and
* \a right_child
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_land(struct ast_node_land *node);

/**
* Destroying function for '||' node (logical or)
* @details Recursively calls destroy_ast ont its element \a left_child and
* \a right_child
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_lor(struct ast_node_lor *node);

/**
* Destroying function for 'for' node
* @details Recursively calls destroy_ast ont its element \a exec and
* free its list.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_for(struct ast_node_for *node);

/**
* Destroying function for 'redirect' node
* @details Recursively calls destroy_ast ont its element \a node.
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_redirect(struct ast_node_redirect *node);

/**
* Destroying function for 'fct_dec' node (function_declaration
* @details Recursively calls destroy_ast ont its element \a function
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_fctdec(struct ast_node_fctdec *node);

/**
* Destroying function for ';' node (semicolon)
* @details Recursively calls destroy_ast ont its element \a left_child and
* \a right_child
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_semicolon(struct ast_node_semicolon *node);

/**
* Destroying function for '!' node (not)
* @details Recursively calls destroy_ast ont its element \a child
* @param node Node to be destroyed. Its type was checked before the call.
*/
void destroy_ast_node_not(struct ast_node_not *node);


/**
* Main function for destroying the AST
* \details Recursively calls sub-destroying functions untils all is erased
* \param node The root of the AST should be passed here.
*/
void destroy_ast(struct ast_node *root);
