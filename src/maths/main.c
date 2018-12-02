#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include "stack.h"

static int digit(char c)
{
    return c >= '0' && c <= '9';
}

static int priority(char op)
{
    switch (op)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            break;
    }
    return 0;
}

static int compute_op(int a, int b, char op)
{
    switch (op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            break;
    }
    errx(1, "libmath : Unknow op %c", op);
}

static int get_number(char *tokens, size_t *i)
{
    int val = 0;
    while(*i < strlen(tokens) && digit(tokens[*i]))
    {
        val = (val * 10) + (tokens[*i] - '0');
        (*i)++;
    }
    (*i)--;
    return val;
}
