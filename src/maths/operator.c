/**
* \file operator.c
* \brief Handle operator arithmetics
* \author thomas.lupin
* \version 1.0
* \date 06-12-2018
**/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <limits.h>

#include "shell.h"
#include "env.h"
#include "stack.h"
#include "env.h"
#include "maths.h"

long long int get_int_len (long long int value)
{
    long long int l = 1;
    if (value < 0)
        value = -value;
    while (value > 9)
    {
        l++;
        value /= 10;
    }
    return l;
}

long long int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

long long int is_var(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c == '_')
        return 1;
    if (c == '$')
        return 1;
    return 0;
}

long long int is_num(char c)
{
    if (is_digit(c))
        return 1;
    if (is_var(c))
        return 1;
    if (c == '!')
        return 1;
    if (c == '~')
        return 1;
    if (c == '-')
        return 1;
    if (c == '+')
        return 1;
    return 0;
}

long long int priority(long long int op)
{
    switch (op)
    {
    case '&':
    case '&' * 2:
    case '|':
    case '|' * 2:
    case '^':
    case '%':
        return 1;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '*' * 2:
        return 4;
    default:
        break;
    }
    return 0;
}

long long int get_operator(char *str, size_t *i, short go_forward)
{
    switch (str[*i])
    {
    case '*':
        if (str[*i + 1] == '*')
        {
            *i += go_forward;
            return '*' * 2;
        }
        return '*';
    case '&':
        if (str[*i + 1] == '&')
        {
            *i += go_forward;
            return '&' * 2;
        }
        return '&';
    case '|':
        if (str[*i + 1] == '|')
        {
            *i += go_forward;
            return '|' * 2;
        }
        return '|';
    default:
        return str[*i];
    }
    return 0;
}

long long int int_pow(long long int num, long long int exp)
{
    long long int res = 1;
    while (exp)
    {
        if (exp & 1)
            res *= num;
        exp /= 2;
        num *= num;
    }
    return res;
}

long long int compute_op_bis(long long int a, long long int b,
                                long long int op)
{
    switch (op)
    {
    case '&':
        return b & a;
    case '&' * 2:
        return b && a;
    case '|':
        return b | a;
    case '|' * 2:

        return b || a;
    case '^':
        return b ^ a;
    case '%':
        return b % a;
    default:
        break;
    }
    warnx("libmath : Malformed input, unexpected %lld", op);
    return INT_MIN;
}

long long int compute_op(long long int a, long long int b,
                                long long int op)
{
    switch (op)
    {
    case '+':
        return b + a;
    case '-':
        return b - a;
    case '*':
        return b * a;
    case '*' * 2:
        return int_pow(b, a);
    case '/':
        if (a == 0)
        {
            warnx("libmath : Cannot divide by 0");
            if (shell.type != S_PROMPT)
                exit(1);
            else
                return 0;
        }
        return b / a;
    default:
        return compute_op_bis(a, b, op);
    }
}
