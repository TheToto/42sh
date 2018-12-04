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

void sort_queue(struct queue *q)
{
    size_t argc = q->size;
    char **argv = q->queue;
    for (size_t i = 0; i < argc; i++)
    {
        for (size_t j = 1; j < (argc - i - 1); j++)
        {
            if (strcmp(argv[j], argv[j + 1]) > 0)
            {
                char *temp = argv[j];
                argv[j] = argv[j + 1];
                argv[j + 1] = temp;
            }
        }
    }
}

char **dump_queue(struct queue *q)
{
    return q->queue;
}

void debug_queue(struct queue *q)
{
    for (size_t i = 0; i < q->size; i++)
        printf("%s ", q->queue[i]);
    printf("\n");
}
