#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>

#include "queue.h"


struct queue *init_queue(void)
{
    struct queue *q = malloc(sizeof(struct queue));
    q->capacity = 8;
    q->queue = malloc(8 * sizeof(char*));
    q->size = 0;
    return q;
}

void destroy_queue(struct queue *q)
{
    for (size_t i = 0; i < q->size; i++)
    {
        free(q->queue[i]);
    }
    free(q->queue);
    free(q);
}

void push_queue(struct queue *q, char *str)
{
    if (q->size == q->capacity)
    {
        q->capacity *= 2;
        q->queue = realloc(q->queue, q->capacity * sizeof(char*));
    }
    q->queue[q->size] = strdup(str);
    q->size++;
}

char **dump_queue(struct queue *q)
{
    return q->queue;
}
