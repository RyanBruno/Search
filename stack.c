/* Ryan Bruno */
#include "stack.h"

void stack_push(struct array* s, void *i)
{
    void **ptr;

    ptr = array_insert(s);
    *ptr = i;
}


void *stack_pop(struct array* s)
{
    void **ptr;

    if (s->array_n <= 0) 
        return NULL;

    ptr = array_get(s, s->array_n - 1);

    s->array_n--;
    return *ptr;
}
