/* Ryan Bruno */
#include "queue.h"

struct queue queue_create(int n)
{
    struct queue q;

    q.queue_front = NULL;
    q.queue_rear = NULL;

    return q;
}

void queue_enqueue(void *v, void *i)
{
    struct queue* q = (struct queue*) v;
    struct queue_item* item;

    item = malloc(sizeof(struct queue_item));
    item->i = i;
    item->next = NULL;

    if (q->queue_rear != NULL) {
        q->queue_rear->next = item;
    }

    q->queue_rear = item;

    if (q->queue_front == NULL) {
        q->queue_front = item;
    }
}

void *queue_dequeue(void* v)
{
    struct queue* q = (struct queue*) v;
    struct queue_item* item;

    item = q->queue_front;

    if (item == NULL) {
        return NULL;
    }

    if (item->next == NULL) {
        q->queue_front = NULL;
        q->queue_rear = NULL;
    } else {
        q->queue_front = item->next;
    }

    // TODO free item
    return item->i;
}
