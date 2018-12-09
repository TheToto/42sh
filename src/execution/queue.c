/**
* \file queue.c
* \brief Queue functions
* \author thomas.lupin
* \version 0.9
* \date 02-12-2018
**/
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "quote_lexer.h"
#include "pathexp.h"


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
    if (q->size == q->capacity - 1)
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
    for (size_t i = 0; i < q->size; i++)
    {
        for (size_t j = 0; j < (q->size - i - 1); j++)
        {
            if (strcasecmp(q->queue[j], q->queue[j + 1]) > 0)
            {
                char *temp = q->queue[j];
                q->queue[j] = q->queue[j + 1];
                q->queue[j + 1] = temp;
            }
        }
    }
}

void sort_queue_alias(struct queue *q)
{
    for (size_t i = 0; i < q->size; i++)
    {
        for (size_t j = 0; j < (q->size - i - 1); j++)
        {
            char *chr1 = strchr(q->queue[j], '=');
            if (chr1)
                *chr1 = 2;
            char *chr2 = strchr(q->queue[j], '=');
            if (chr2)
                *chr2 = 2;
            if (strcmp(q->queue[j], q->queue[j + 1]) > 0)
            {
                char *temp = q->queue[j];
                q->queue[j] = q->queue[j + 1];
                q->queue[j + 1] = temp;
            }
            if (chr1)
                *chr1 = '=';
            if (chr2)
                *chr2 = '=';

        }
    }
}

char **dump_queue(struct queue *q)
{
    q->queue[q->size] = NULL;
    char **res = q->queue;
    free(q);
    return res;
}

void debug_queue(struct queue *q)
{
    for (size_t i = 0; i < q->size; i++)
        printf("%s ", q->queue[i]);
    printf("\n");
}

static size_t get_total_len(struct queue *q)
{
    size_t acu = 0;
    for (size_t i = 0; i < q->size; i++)
    {
        acu += strlen(q->queue[i]) + 1;
    }
    return acu;
}

char *concat_quote(char *value)
{
    struct queue *q = init_queue();
    remove_quoting(value, q);
    char *res = calloc(get_total_len(q) + 1, sizeof(char));
    char **dump = dump_queue(q);
    for (size_t i = 0; dump[i]; i++)
    {
        dump[i] = remove_backslash(dump[i]);
        strcat(res, dump[i]);
        if (dump[i + 1])
            strcat(res, " ");
    }
    for (size_t i = 0; dump[i]; i++)
        free(dump[i]);
    free(dump);
    return res;
}

char *concat_case(char *value)
{
    struct queue *q = init_queue();
    remove_quoting(value, q);
    char *res = calloc(get_total_len(q) + 1, sizeof(char));
    char **dump = dump_queue(q);
    for (size_t i = 0; dump[i]; i++)
    {
        strcat(res, dump[i]);
        if (dump[i + 1])
            strcat(res, " ");
    }
    for (size_t i = 0; dump[i]; i++)
        free(dump[i]);
    free(dump);
    return res;
}
