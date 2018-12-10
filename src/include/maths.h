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

/// Get the length of a long long int
long long int get_int_len (long long int value);

/// Return true if it's a digit
long long int is_digit(char c);

/// Return true if it's a var
long long int is_var(char c);

/// Return true is a number
long long int is_num(char c);

/// Return the priority of \a op
long long int priority(long long int op);

/// Return the next operator
long long int get_operator(char *str, size_t *i, short go_forward);

/// Compute pow operator
long long int int_pow(long long int num, long long int exp);

/// Compute the next \a op in the stack
long long int compute_op_bis(long long int a, long long int b,
                                long long int op);

/// Compute the next \a op in the stack
long long int compute_op(long long int a, long long int b,
                                long long int op);
