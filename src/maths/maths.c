#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include "stack.h"

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int is_num(char c)
{
    return (c >= '0' && c <= '9') || c == '+' || c == '-';
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
    errx(1, "libmath : Malformed input, unexpected %c", op);
}

static int get_number(char *str, size_t *i)
{
    int val = 0;
    int less = 0;
    if (str[*i] == '-')
    {
        less = 1;
        (*i)++;
    }
    else if (str[*i] == '+')
        (*i)++;
    for (; *i < strlen(str) && str[*i] == ' '; (*i)++);
    while (*i < strlen(str) && is_digit(str[*i]))
    {
        val = (val * 10) + (str[*i] - '0');
        (*i)++;
    }
    (*i)--;
    return less ? -val : val;
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
    int left = pop_stack(values);
    int right = pop_stack(values);
    char op = pop_stack(sign);
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
    return 0;
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
        else if (priority(str[i]) != 0 && !want_num)
        {
            while (!is_empty_stack(sign)
                    && priority(peak_stack(sign)) >= priority(str[i]))
            {
                compute_next(values, sign);
            }
            push_stack(sign, str[i]);
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

int main(void)
{
    return evaluate_maths("- 3 +(3+2)*2");
}
