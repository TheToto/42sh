/**
* \file options.h
* Parse the options with the format [GNU long option] [option] script-file
* \author sabrina.meng thomas.lupin
* \version 0.9
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

/**
* Get the option type according to the enum option of the header
* \param char *opt  The option to check
* \return Return an enum value according to the option
*/
enum option get_option(char *opt);

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

/**
* Show PS2 for quoting, and add it next to shell.buf
* \return Return next string pointer
*/
char *quote_ps2(void);
