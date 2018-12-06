#pragma once

struct stack
{
    size_t size;
    size_t capacity;
    long long int* tab;
};

struct stack *init_stack(void);
void destroy_stack(struct stack *stack);
long long int is_full_stack(struct stack *stack);
long long int is_empty_stack(struct stack *stack);
void push_stack(struct stack *stack, long long int item);
long long int pop_stack(struct stack* stack);
long long int peak_stack(struct stack* stack);
