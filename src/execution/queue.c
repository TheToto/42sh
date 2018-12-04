#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void fusion_queue(struct queue *dest, struct queue *src)
{
    for (size_t i = 0; i < src->size; i++)
        push_queue(dest, src->queue[i]);
    destroy_queue(src);
}

static int my_strcmp(const void *p1, const void *p2)
{
    const char **str1 = p1;
    const char **str2 = p2;
    return strcmp(*str1, *str2);
}

void sort_queue(struct queue *q)
{
    qsort(q->queue, q->size, sizeof(char*), my_strcmp);
}

char **dump_queue(struct queue *q)
{
    return q->queue;
}

void debug_queue(struct queue *q)
{
    for (size_t i = 0; i < q->size; i++)
        printf("%s -> ", q->queue[i]);
    printf("END\n");
}
