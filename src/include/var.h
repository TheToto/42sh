/**
 * \file var.h
 * \author louis.holleville
 * \version 0.3
 * \date 16-11-2018
 * \brief Management of shell variables header
 */

#include <stddef.h>

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

struct variables *init_var(void);
void add_var(struct variables *var, char *name, char *value);
void destroy_var(struct variables *var);
char *get_var(struct variables *var, char *name);