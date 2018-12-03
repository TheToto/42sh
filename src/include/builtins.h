/**
 * \file builtins.h
 * Execute the builtins
 * \authors sabrina.meng
 * \version 0.8
 * \date 30-11-2018
**/
#pragma once

#define PATH_MAX 4096

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
    HISTORY
};

/**
 * Get the nb of arguments in the string given
 * \param char **str  The string to be parsed
 * \return Return the nb of arguments
**/
size_t get_args(char **str);

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
