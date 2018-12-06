/**
* \file queue.h
* Queue functions
* \author thomas.lupin
* \version 0.8
* \date 06-12-2018
**/
#pragma once

struct queue
{
    char **queue;
    size_t capacity;
    size_t size;
};

struct queue *init_queue(void);
void destroy_queue(struct queue *q);
void push_queue(struct queue *q, char *str);
char **dump_queue(struct queue *q);
void sort_queue(struct queue *q);
void sort_queue_alias(struct queue *q);
void fusion_queue(struct queue *dest, struct queue *src);
void debug_queue(struct queue *q);
char *concat_quote(char *value);
