/* Ryan Bruno */
#include "array.h"

void array_create(struct array* array_r, size_t s, int c)
{
    array_r->array_c = c;
    array_r->array_n = 0;
    array_r->array_s = s;
    array_r->array_data = (char*) malloc(s * c);

    memset(array_r->array_data, 0, s * c);
}

void* array_insert(struct array* a) 
{
    void *ptr;

    if (a->array_n >= a->array_c) {

        a->array_c *= 1.5;
        a->array_data = realloc(a->array_data, a->array_s * a->array_c);

        if (a->array_data == NULL) {
            printf("Resize Failed!\n");
            return NULL;
        }
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

void array_reset(struct array* a)
{
    a->array_n = 0;
}
