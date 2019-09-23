#include "array.h"

/* Pointer only heap */
typedef struct array heap;

struct heap_item {
    void *heap_item;
    float heap_priority;
};

heap heap_create(int n);
void heap_insert(heap* h, void* item, float priority);
void *heap_pop(heap* h);
void array_swap_index(struct array* arr, int a, int b);
