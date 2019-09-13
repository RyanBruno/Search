/* Ryan Bruno */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <string.h>
#include "array.h"

struct queue_item {
    void *item;
    struct queue_item *next;
};

typedef struct array queue;

struct queue *queue_create();
void queue_enqueue(struct array* q, void *i);
void *queue_dequeue(struct array* q);

#endif
