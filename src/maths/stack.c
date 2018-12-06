#include <stdlib.h>
#include <err.h>
#include <limits.h>

#include "stack.h"

struct stack *init_stack(void)
{
    struct stack *stack = malloc(sizeof(struct stack));
    if (!stack)
        errx(1, "libstack : Failled to malloc stack");
    stack->capacity = 10;
    stack->size = 0;
    stack->tab = malloc(stack->capacity * sizeof(int));
    if (!stack->tab)
        errx(1, "libstack : Failled to malloc stack");
    return stack;
}

void destroy_stack(struct stack *stack)
{
    free(stack->tab);
    free(stack);
}

long long int is_full_stack(struct stack *stack)
{
    return stack->size == stack->capacity;
}

long long int is_empty_stack(struct stack *stack)
{
    return stack->size == 0;
}

static void expand_stack(struct stack *stack)
{
    long long int *new_tab = realloc(stack->tab,
            stack->capacity * 2 * sizeof(long long int));
    if (!new_tab)
        errx(1, "libstack : Failled to realloc stack");
    stack->capacity *= 2;
    stack->tab = new_tab;
}

void push_stack(struct stack *stack, long long int item)
{
    if (is_full_stack(stack))
        expand_stack(stack);
    long long int to_push = stack->size;
    stack->size += 1;
    stack->tab[to_push] = item;
}

long long int pop_stack(struct stack* stack)
{
    if (is_empty_stack(stack))
        return INT_MIN;
    long long int to_remove = stack->size - 1;
    stack->size -= 1;
    return stack->tab[to_remove];
}

long long int peak_stack(struct stack* stack)
{
    if (is_empty_stack(stack))
        return INT_MIN;
    long long int to_peak = stack->size - 1;
    return stack->tab[to_peak];
}
