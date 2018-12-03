/**
* \file env.h
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
    char exported;  ///<attribute exported
};

///enumeration of different types of functions
enum f_type
{
    BUILTIN = 0,///<function is a builtin
    DECLARED = 1///<function as been declared
};

/// definition of a builtin or a declared functions
struct func
{
    char *name;         ///<name of the expansion
    void *value;        ///<value of the expansion
    enum f_type type;   ///<type of the function
};

/**
* @brief Array of all shell defined variables
*/
struct variables
{
    size_t size;        ///<size of the var array
    size_t capacity;    ///<capacity of the var array
    struct var **lib;   ///<array of defined variables and their value
    size_t f_size;      ///<size of the functions array
    size_t f_capacity;  ///<capacity of the functions array
    struct func **f_lib;///<array of defined functions or builtins
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
void add_var(struct variables *var, char *name, char *value, char exported);

/**
* @fn destroy_var
* Destroy array of variables and all incorporated variables.
* @param var pointer to the library of variables to destroy
*/
void destroy_var(struct variables *var);

/**
 * @fn del_var
 * Remove the name variables from the library if exists
 * @param var pointer to the library of variables to destroy
 * @param name variable to remove
 */
void del_var(struct variables *var, char *name);

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
* @param scmd new variable
* Replace element by its new variable if a declaration was made
*/
char **replace_var_scmd(struct ast_node_scmd *scmd);

/**
* @param var library to use
* @param name name of the function
* @param value value to store as a function
* @param type type of assignements (DECLARED or BUILTIN)
* Add the new function to the library of functions
*/
int add_func(struct variables *var, char *name, void *value, enum f_type type);

/**
* Look for the variable with name \a name in the array of functions
* @param var library to look in
* @param name variable researched
* @return NULL if not found, its value otherwise
*/
void *get_func(struct variables *var, char *name);

void set_up_var(char *args[]);
