#pragma once

/**
 *\brief Enumeration of the different options
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
 *\brief Enumeration of the shopt variables
 */
enum shopt
{
    OTHER = 0,
    ASTPRINT,
    DOTGLOB,
    EXP_ALIAS,
    EXTGLOB,
    NOCASEGLOB,
    NULLGLOB,
    SRCPATH,
    XPGECHO,
};

enum option get_option(char *opt);
enum shopt get_shopt(char *arg);
void options(char *argv[]);
