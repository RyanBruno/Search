/* Ryan Bruno */
#include "array.h"

struct array
array_create(size_t s, int c)
{
    struct array a;

    a.array_c = c;
    a.array_n = 0;
    a.array_s = s;
    a.array_data = (char*) malloc(s * c);

    memset(a.array_data, 0, s * c);

    return a;
}

void* array_insert(struct array* a) 
{
    void *ptr;

    if (a->array_n >= a->array_c) {
        // Resize by c * 1.5
        // Also print to debug
        printf("Resize Needed!\n");
        return NULL;
    }

    ptr = &(a->array_data[a->array_n * a->array_s]);
    a->array_n++;
    return ptr;
}

void* array_get(struct array* a, int n)
{
    if (n < 0 || n >= a->array_n) {
        return NULL;
    }

    return &(a->array_data[n * a->array_s]);
}

int array_size(struct array* a)
{
    return a->array_n;
}
