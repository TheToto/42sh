/**
* \file queue.h
* \brief Queue functions
* \author thomas.lupin
* \version 0.9
* \date 06-12-2018
**/
#pragma once

/// The struct of the queue
struct queue
{
    char **queue; ///<The array of all elements
    size_t capacity; ///<The capacity of the queue
    size_t size; ///<The current size of the queue
};

/**
* init the queue with a capacity of 8
*/
struct queue *init_queue(void);

/**
* Destroy the queue \a q
*/
void destroy_queue(struct queue *q);

/**
* Push a element in the queue
*/
void push_queue(struct queue *q, char *str);

/**
* Return a mallocated null terminated list and free the queue \a q
*/
char **dump_queue(struct queue *q);

/**
* Sort all elements in the queue \a q
*/
void sort_queue(struct queue *q);

/**
* Sort all elements in the queue \a q (case sensitive)
*/
void sort_queue_alias(struct queue *q);

/**
* Concat two queue
*/
void fusion_queue(struct queue *dest, struct queue *src);

/**
* Print the queue to \a stdout
*/
void debug_queue(struct queue *q);

/**
* Call remove quotes and concat the result using a queue
*/
char *concat_quote(char *value);

/**
* Call remove quotes and concat the result using a queue (with backslash)
*/
char *concat_case(char *value);
