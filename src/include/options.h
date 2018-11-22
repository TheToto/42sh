/**
 *\file options.h
 *\author sabrina.meng thomas.lupin
 *\version 0.5
 *\date 15-11-2018
 *\brief Options parsing
 *\detail Parse the options with the format
 *[GNU long option] [option] script-file
 */
 #pragma once

/**
 *\brief Enumeration of the different options
 */
enum option
{
    NONE = 0,///<Unknown or no options
    SHOPT_MINUS,///<-O     Disables shopt variables
    SHOPT_PLUS,///<+O      Enables shopt variables
    CMD,///<-c <command>   Executes the command
    NORC,///<--norc        Deactivates ressource reader
    AST,///<--ast-print    Prints the AST
    VERSION,///<--version  Prints the current version on the standard output
};

/**
 *\brief Enumeration of the shopt variables
 */
enum shopt
{
    OTHER = 0,
    NO,
    ASTPRINT,
    DOTGLOB,
    EXP_ALIAS,
    EXTGLOB,
    NOCASEGLOB,
    NULLGLOB,
    SRCPATH,
    XPGECHO,
};

/**
 *\fn get_option
 *\brief Get the option type according to the enum option of the header
 *\param char *opt  The option to check
 *\return Return an enum value according to the option
 */
enum option get_option(char *opt);

/**
 *\fn get_shopt
 *\brief Get the shopt variable according to the enum shopt of the header
 *\param char *arg  The shopt variable to check
 *\return Return an enum value according to the shopt variable
 */
enum shopt get_shopt(char *arg);

/**
 *\fn options
 *\brief Do actions according to each options
 *\param char *argv[]   The command line to parse
 *\return The return value depends on the options
 */
void options(char *argv[]);

/**
 *\fn show_prompt
 *\brief Show the prompt in a infinite loop
 *\param int norc boolean to load .rc files
 *\param int is_print boolean to print ast
 *\return Return last operation result
 */
int show_prompt(int norc, int is_print);
