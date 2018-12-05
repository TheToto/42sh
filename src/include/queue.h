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
void fusion_queue(struct queue *dest, struct queue *src);
void debug_queue(struct queue *q);
char *concat_quote(char *value);