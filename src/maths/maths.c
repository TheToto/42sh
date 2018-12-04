#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <limits.h>

#include "shell.h"
#include "env.h"
#include "stack.h"

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int is_num(char c)
{
    if (is_digit(c))
        return 1;
    if (c == '!')
        return 1;
    if (c == '~')
        return 1;
    if (c == '-')
        return 1;
    return 0;
}

static int priority(int op)
{
    switch (op)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '*' * 2:
        case '&' * 2:
        case '|':
        case '|' * 2:
        case '^':
            return 3;
        default:
            break;
    }
    return 0;
}

static int get_operator(char *str, size_t *i, short go_forward)
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

static int int_pow(int num, int exp)
{
    int res = 1;
    while (exp)
    {
        if (exp & 1)
            res *= num;
        exp /= 2;
        num *= num;
    }
    return res;
}

static int compute_op(int a, int b, int op)
{
    switch (op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '*' * 2:
            return int_pow(b, a);
        case '/':
            return a / b;
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
        default:
            break;
    }
    warnx("libmath : Malformed input, unexpected %d", op);
    return INT_MIN;
}

static int apply_modif(char modif, int val)
{
    if (modif == '!')
        return val == 0 ? 1 : 0;
    if (modif == '~')
        return ~val;
    if (modif == '-')
        return -val;
    warnx("libmath : Unknow modifier %c", modif);
    return INT_MIN;
}

static int get_number(char *str, size_t *i)
{
    int val = 0;
    size_t j = *i;
    while (*i < strlen(str) && !is_digit(str[*i]))
        (*i)++;
    while (*i < strlen(str) && is_digit(str[*i]))
    {
        val = (val * 10) + (str[*i] - '0');
        (*i)++;
    }
    (*i)--;
    while (j < strlen(str) && !is_digit(str[j]))
    {
        val = apply_modif(str[j], val);
        if (val == INT_MIN)
            return val;
        j++;
        for (; j < strlen(str) && str[j] == ' '; j++);
    }
    return val;
}

static int destroy_maths(struct stack *values, struct stack *sign)
{
    int res = peak_stack(values);
    destroy_stack(values);
    destroy_stack(sign);
    return res;
}

static void compute_next(struct stack *values, struct stack *sign)
{
    int left = INT_MIN;
    int right = INT_MIN;
    int op = INT_MIN;
    left = pop_stack(values);
    right = pop_stack(values);
    op = pop_stack(sign);
    if (right == INT_MIN || left == INT_MIN)
    {
        warnx("libmath : Operators or values stack is empty");
        push_stack(values, INT_MIN);
    }
    else
        push_stack(values, compute_op(left, right, op));
}

static int math_error(char *str, int want_num,
        struct stack *values, struct stack *sign)
{
    destroy_stack(values);
    destroy_stack(sign);
    if (want_num)
        warnx("libmath : invalid number '%s'", str);
    else
        warnx("libmath : invalid operator '%s'", str);
    return INT_MIN;
}


int evaluate_maths(char *str)
{
    struct stack *values = init_stack();
    struct stack *sign = init_stack();
    int want_num = 1;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
            continue;
        else if (str[i] == '(' && want_num)
            push_stack(sign, str[i]);
        else if (str[i] == ')' && !want_num)
        {
            while (!is_empty_stack(sign) && peak_stack(sign) != '(')
                compute_next(values, sign);
            pop_stack(sign);
        }
        else if (is_num(str[i]) && want_num)
        {
            push_stack(values, get_number(str, &i));
            want_num = 0;
        }
        else if (priority(get_operator(str, &i, 0)) != 0 && !want_num)
        {
            while (!is_empty_stack(sign) && priority(peak_stack(sign))
                    >= priority(get_operator(str, &i, 0)))
            {
                compute_next(values, sign);
            }
            push_stack(sign, get_operator(str, &i, 1));
            want_num = 1;
        }
        else
        {
            return math_error(str + i, want_num, values, sign);
        }
    }
    while (!is_empty_stack(sign))
        compute_next(values, sign);
    return destroy_maths(values, sign);
}

int get_int_len (int value)
{
    int l = 1;
    while(value > 9)
    {
        l++;
        value /= 10;
    }
    return l;
}

char *get_maths(char *str)
{
    int res = evaluate_maths(str);
    char *ret = calloc(get_int_len(res) + 2, sizeof(char));
    sprintf(ret, "%d", res);
    add_var(shell.var, "RESERVED_MATH", ret, 0);
    free(ret);
    return get_var(shell.var, "RESERVED_MATH");
}
