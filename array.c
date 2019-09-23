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

        printf("Resized\n");
        a->array_c *= 1.5;
        a->array_data = realloc(a->array_data, a->array_s * a->array_c);

        if (a->array_data == NULL) {
            printf("Resize Failed!\n");
            return NULL;
        }
    } else {
        printf("Not Resized\n");
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

void *array_get_end(struct array* a)
{
    void *ptr;

    ptr = array_get(a, a->array_n - 1);

    return ptr;
}

void *array_pop(struct array* a)
{
    void *ptr;

    if (a->array_n <= 0) 
        return NULL;

    ptr = array_get_end(a);

    a->array_n--;
    return ptr;
}

int array_size(struct array* a)
{
    return a->array_n;
}

int array_full(struct array* a)
{
    return a->array_n >= a->array_c;
}
