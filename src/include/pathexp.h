/**
* \file pathexp.h
* \brief Handle the path expansion
* \author thomas.lupin
* \version 1.0
* \date 06-12-2018
**/
#pragma once

/**
* \fn expand_path
* Expand the path with globbing
* \param path  The string to expand
* \return Return a queue with globbing result
*/
struct queue *expand_path(char *path);

/**
* \fn remove_backslash
* Remove backshash from a string
* \param old  The string to perferm
* \return Return a string without backslash
*/
char *remove_backslash(char *old);
