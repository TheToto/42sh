#pragma once

struct stack
{
    size_t size;
    size_t capacity;
    int* tab;
};

struct stack *init_stack(void);
int is_full_stack(struct stack *stack);
int is_empty_stack(struct stack *stack);
void push_stack(struct stack *stack, int item);
int pop_stack(struct stack* stack);
int peak_stack(struct stack* stack);
void destroy_stack(struct stack *stack);
