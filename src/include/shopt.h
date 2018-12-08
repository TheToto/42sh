/**
* \file shopt.h
* Handle the shopt variables
* \authors sabrina.meng
* \version 0.9
* \date 28-11-2018
*/
#pragma once

#define NB_SHOPT 8

///Enumeration of the shopt variables
enum shopt
{
    ASTPRINT = 0,///<ast_print  Prints the AST

    DOTGLOB,/**<dotglob  Bash includes filenames beginning with a ‘.’
                         in the results of filename expansion.*/

    EXP_ALIAS,///<expand_aliases  Aliases are expanded

    EXTGLOB,///<extglob  The extended pattern matching features are enabled

    NOCASEGLOB,/**<nocaseglob  Bash matches filenames in a case-insensitive
                               fashion when performing filename expansion.*/

    NULLGLOB,/**<nullglob  Bash allows filename patterns which match no files
                           to expand to a null string, rather than themselves.
                           */

    SRCPATH,/**<sourcepath  The source builtin uses the value of PATH to find
                            the directory containing the file supplied as
                            an argument.*/

    XPGECHO,/**<xpg_echo  The echo builtin expands backslash-escape sequences
                          by default.*/

    OTHER, ///<Not a shopt variable

    NO,///<No arguments given to shopt builtin
};

/**
* Initialize the list of shopt variables and their state
* \return Return the shopt list
*/
int *init_shoptlist(void);

/**
* Get the shopt variable according to the enum shopt of the header
* \param char *arg  The shopt variable to check
* \return Return an enum value according to the shopt variable
*/
enum shopt get_shopt(char *arg);

/**
* Prints an error message on stderr in case of wrong usage of [-+]O
* \return Return 2
*/
int err_shopt(void);

/**
* Prints the state of the shopt variables
* \param int is_builtin  Check if this function is used by the shopt builtin
* \param enum shopt shopt  The shopt variable if given
*/
void print_shopt(int is_builtin, enum shopt shopt);

/**
* Prints the commands run to set the shopt variables to their default state
* \param enum shopt shopt  The shopt variable if given
*/
void print_shopt_plus(enum shopt shopt);

/**
* Execute the shopt builtin
* \param char **str  The line command with the shopt builtin
* \return Return 0 if no error, else return a value different from 0
*/
int shopt_exec(char **str);

/**
* Update the $SHELLOPTS variable
**/
void update_shellopts(void);
