/**
* \file stack.h
* \brief Stack functions
* \author thomas.lupin
* \version 1.0
* \date 06-12-2018
**/
#pragma once

/// The stack struct
struct stack
{
    size_t size; ///<The size of the stack
    size_t capacity; ///<The max capacity of the stack
    long long int* tab; ///<The array of the stack
};

/// Initialize a stack with a capacity of 8
struct stack *init_stack(void);

/// Destroy the stack \a stack
void destroy_stack(struct stack *stack);

/// Test if the stack is full
long long int is_full_stack(struct stack *stack);

/// Test if the stack is empty
long long int is_empty_stack(struct stack *stack);

/// Push \a item in the stack \a stack
void push_stack(struct stack *stack, long long int item);

/// Remove a item from the stack \a stack
long long int pop_stack(struct stack* stack);

/// Return the next item to pop from stack \a stack
long long int peak_stack(struct stack* stack);
