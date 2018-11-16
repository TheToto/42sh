/**
 * \file var.c
 * \author louis.holleville
 * \version 0.3
 * \date 16-11-2018
 * \brief Management of shell variables
 */

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "var.h"

/**
 * @fn init_var
 * @brief Initialization of the array of variables
 * @details malloc the structure and malloc an array of an initial size of 8.
 * @return return the newly allocated structure
 */
struct variables *init_var(void)
{
    struct variables *new = malloc(sizeof(struct variables));
    if (!new)
    {
        errx(1, "cannot malloc in init_var");
        return NULL;
    }
    struct var **lib = calloc(8, sizeof(struct lib*));
    if (!lib)
    {
        errx(1, "cannot malloc in init_var");
        free(new);
        return NULL;
    }
    new->lib = lib;
    new->size = 0;
    new->capacity = 8;
    return new;
}

static void expand_var(struct variables *var)
{
    struct var **new = realloc(var->lib, 2 * var->capacity
            * sizeof(struct variables*));
    if (!new)
        errx(1,"cannot realloc in expand_var");
    var->lib = new;
    var->capacity *= 2;
}

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

void add_var(struct variables *var, char *name, char *value)
{
    if (!var || !name || !value)
    {
        warnx("cannot add_var (name = %s, value = %s", name, value);
        return;
    }
    size_t pos = 0;
    struct var *cur = var->lib[pos];
    int found = 1;
    for (; found && pos < var->size; pos++, cur = var->lib[pos])
    {
        if (strcmp(cur->name, name) == 0)
            found = 0;
    }

    if (found)
    {
        cur->value = value;
        return;
    }
    if (pos == var->capacity)
        expand_var(var);
    struct var *new = malloc(sizeof(struct var));
    if (!var)
        errx(1, "cannot malloc new word in add_var");
    new->name = strdup(name);
    new->value = strdup(value);
    var->size += 1;
    var->lib[pos] = new;
}


/**
 * @fn destroy_var
 * @brief Destroy array of variables and all incorporated variables.
 */
void destroy_var(struct variables *var)
{
    if (!var)
    {
        warnx("cannot destroy_var: no lib provided");
        return;
    }
    struct var *cur;
    for (size_t i = 0; i < var->size; i++)
    {
        cur = var->lib[i];
        free(cur->name);
        free(cur->value);
        free(cur);
    }
    free(var->lib);
    free(var);
}

/**
 * @fn get_var
 * @brief Look for the variable with name \a name in the array \var
 * @return NULL if not found, its value otherwise
 */
char *get_var(struct variables *var, char *name)
{
    if (!var || !name)
    {
        warnx("cannot get_var: no var or name provided");
        return NULL;
    }
    struct var *cur;
    size_t i = 0;
    for (; i < var->size; i++)
    {
        cur = var->lib[i];
        if (strcmp(name, cur->name) == 0)
            break;
    }
    if (i == var->size)
        return NULL;
    return cur->value;
}
