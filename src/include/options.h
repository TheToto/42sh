/**
* \file options.h
* Parse the options with the format [GNU long option] [option] script-file
* \author sabrina.meng thomas.lupin
* \version 0.5
* \date 27-11-2018
*/
 #pragma once

///Enumeration of the different options
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

///Enumeration of the shopt variables
enum shopt
{
    OTHER = 0, ///<Not a shopt variable

    NO,///<No arguments given to shopt builtin

    ASTPRINT,///<ast_print  Prints the AST

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
};

/**
* Get the option type according to the enum option of the header
* \param char *opt  The option to check
* \return Return an enum value according to the option
*/
enum option get_option(char *opt);

/**
* Get the shopt variable according to the enum shopt of the header
* \param char *arg  The shopt variable to check
* \return Return an enum value according to the shopt variable
*/
enum shopt get_shopt(char *arg);

/**
* Do actions according to each options
* \param char *argv[]   The command line to parse
*/
void options(char *argv[]);

/**
* Show the prompt in a infinite loop
* \param int norc boolean to load .rc files
* \param int is_print boolean to print ast
* \return Return last operation result
*/
int show_prompt(int norc, int is_print);

/**
 * Show the PS2 prompt and return new lexer
 * \return Return new token_list
 */
struct token_list *show_ps2(void);
