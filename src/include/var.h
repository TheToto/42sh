/**
 * \file var.h
 * \author louis.holleville
 * \version 0.3
 * \date 16-11-2018
 * \brief Management of shell variables header
 */

#pragma once

#include <stddef.h>

#include "ast.h"

/**
 * @brief definition of a variable
 */
struct var
{
    char *name;     ///<variable name
    char *value;    ///<variable value
};

/**
 * @brief Array of all shell defined variables
 */
struct variables
{
    size_t size;        ///<size of the array
    size_t capacity;    ///<capacity of the array
    struct var **lib;   ///<array of defined variables and their value
};

/**
 * @fn init_var
 * @brief Initialization of the array of variables
 * @details malloc the structure and malloc an array of an initial size of 8.
 * @return return the newly allocated structure
 */
struct variables *init_var(void);

/**
 * @fn add_var
 * @brief Add a new variable to the array
 * @details Look if a variable with the same name already exist and replace its
 * value when appropriated. Otherwise, realloc if size too small and allocate
 * the definition at first available cell in the array.
 * @param var the pre-created array of definitions
 * @param name the name of the variable
 * @param value the value of the variable
 * @return return the newly allocated structure
 */
void add_var(struct variables *var, char *name, char *value);

/**
 * @fn destroy_var
 * Destroy array of variables and all incorporated variables.
 * @param var pointer to the library of variables to destroy
 */
void destroy_var(struct variables *var);

/**
 * @fn get_var
 * Look for the variable with name \a name in the array \var
 * @param var library to look in
 * @param name variable researched
 * @return NULL if not found, its value otherwise
 */
char *get_var(struct variables *var, char *name);

/**
 * @fn assign_prefix
 * @param var library to be completed
 * @param prefix prefix to be added
 * Add the declararion to the array of variables
 */
void assign_prefix(struct variables *var, char *prefix);

/**
 * @fn replace_var
 * @param var library to use
 * @param scmd new variable
 * Replace element by its new variable if a declaration was made
 */
char **replace_var_scmd(struct variables *var, struct ast_node_scmd *scmd);
