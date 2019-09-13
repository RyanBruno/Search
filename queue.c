/* Ryan Bruno */
#include "queue.h"

void queue_enqueue(struct array *q, void *i)
{
    void **ptr;

    ptr = array_insert(q);
    *ptr = i;
}

void *queue_dequeue(struct array* q)
{
    void **ptr;
    int *head;

    if (q->array_n <= 0) 
        return NULL;

    head = (int*) array_get(q, 0);
    (*head)++;
    ptr = array_get(q, *head);

    return *ptr;
}
