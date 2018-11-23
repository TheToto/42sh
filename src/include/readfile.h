/**
 *\file readfile.h
 *\author thomas.lupin
 *\version 0.5
 *\date 22-11-2018
 *\brief Header of readfile
 */

/**
 *\fn launch_file
 *\brief Read the whole file \b path and send it to \b exec_main
 *\param path Path to file
 *\param is_print boolean to print the ast
 */
int launch_file(char *path, int is_print);

/**
 *\fn launch_pipe
 *\brief Read \b stdin to handle pipe in 42sh
 *\param is_print boolean to print the ast
 */
int launch_pipe(int is_print);