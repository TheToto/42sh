/**
* \file builtins.h
* \brief Execute the builtins
* \authors sabrina.meng louis.holleville
* \version 1.0
* \date 30-11-2018
**/
#pragma once

#define PATH_MAX 4096

#include <stddef.h>

///Enumeration of the builtins
enum builtin
{
    ANY = 0,
    EXIT,
    CD,
    SHOPT,
    EXPORT,
    ALIAS,
    UNALIAS,
    ECHO,
    CONTINUE,
    BREAK,
    SOURCE,
    HISTORY,
    READ
};

/**
* Get the nb of arguments in the string given
* \param char **str  The string to be parsed
* \return Return the nb of arguments
**/
size_t get_args(char **str);

/**
* Check if a string is only composed of 0
* \param char *s  The string to check
* \return Return 1 if it is 0, 0 otherwise
**/
int is_zero(char *s);

/**
* Check if it is a builtin then execute it
* \param char **str  The string to be parsed
* \return Return 0 if no error, else, a value different from 0
**/
int exec_builtin(char **str);

/**
* Execute the exit builtin
* \param char **str  The string to be parsed
* \return Return the value specified in arguments or if not, return the value
* of the last command
**/
int exec_exit(char **str);

/**
* Execute the cd builtin
* \param char **str  The string to be parsed
* \return Return 1 in case of error, else 0
**/
int changedir(char **str);

/**
* Execute the echo builtin
* \param char **str  The string to be parsed
**/
int echo(char **str);


/**
* Execute the export builtin
* \param char **str  The string to be parsed
**/
int exec_export(char **str);

/**
* Execute the source | '.' builtins
* \param char **str  The string to be parsed
* \return Return 2 if no filename is given,
* 1 if filename doesn't exist, else 0
**/
int source(char **str);

/**
* Execute the continue builtin
* \param char **str  The string to be parsed
**/
int exec_continue(char **str);

/**
* Execute the break builtin
* \param char **str  The string to be parsed
**/
int exec_break(char **str);

/**
* Execute the history builtin
* \param char **str  The string to be parsed
**/
int exec_history(char **str);

/**
* Execute the alias builtin
* @param str the string to be parsed
* @return exit status of the builtin
*/
int exec_alias(char **str);

/*
* Execute the unalias builtin
* @param str the string to be parsed
* @return exit status of the builtin
*/
int exec_unalias(char **str);

/*
* Execute the read builtin
* @param str the string to be parsed
* @return exit status of the builtin
*/
int exec_read(char **str);
