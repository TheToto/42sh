#pragma once

/**
 *\brief enum of the different options
 *\details List all the options handled by the 42sh
 */
enum option
{
    NONE = 0,///<Unknown or no options
    SHOPT,///<[-+]O        Enables shopt variables
    CMD,///<-c <command>   Executes the command
    NORC,///<--norc        Deactivates ressource reader
    AST,///<--ast-print    Prints the AST
    VERSION,///<--version  Prints the current version on the standard output
};

/**
 *\fn options
 *\brief Do actions according to each options
 *\param char *argv[]   The command line to parse
 *\return The return value depends on the options
 */
void options(char *argv[]);
