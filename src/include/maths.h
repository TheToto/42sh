/**
* \file maths.h
* \brief Handle arithmetics
* \author thomas.lupin
* \version 1.0
* \date 06-12-2018
**/
#pragma once

/**
* \fn evaluate_maths
* Compute mathematics expression
* \param str  The string to compute
* \return Return a long long int : the result
*/
long long int evaluate_maths(char *str);

/**
* \fn get_maths
* Compute mathematics expression
* \param str  The string to compute
* \return Return a string containing the result
*/
char *get_maths(char *str);
