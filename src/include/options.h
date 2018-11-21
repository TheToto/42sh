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

enum option get_option(char *opt);
enum shopt get_shopt(char *arg);
void options(char *argv[]);
int show_prompt(int norc, int is_print);
