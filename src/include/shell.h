/**
*\file shell.h
*\author thomas.lupin
*\version 0.8
*\date 22-11-2018
*Header of shell global var
*/

/**
* Read the whole file \b path and send it to \b exec_main
* \param path Path to file
* \param is_print boolean to print the ast
*/

#pragma once

///Shell type enum
enum shell_type
{
    S_FILE,     ///<42sh will execute a file
    S_PROMPT,   ///<42sh will display a prompt
    S_INPUT,    ///<42sh will read stdin
    S_OPTION,   ///<42sh will execute -c option
    S_UNDEFINED ///<Security
};

///Shell global struct
struct shell
{
    enum shell_type type;   ///<Shell type enum
    struct lexer *lexer;    ///<Current lexer
    struct variables *var;  ///<Current var library
    struct ast_node *ast;   ///<Current ast
    struct aliases *alias;  ///<Current alias library
    char *buf;              ///<Current buffer
    int *shopt_states;      ///<Current shopt vars
    int loop;               ///<Current number of loops
    int n_break;            ///<Number of break needed
    int n_continue;         ///<Number of continue needed
};

///Shell global struct
extern struct shell shell;
