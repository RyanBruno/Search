/* Ryan Bruno */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <string.h>

struct queue_item {
    void *i;
    struct queue_item* next;
};

struct queue {
    struct queue_item* queue_front;
    struct queue_item* queue_rear;
};

struct queue queue_create();
void queue_enqueue(void *v, void *i);
void *queue_dequeue(void* v);

#endif
