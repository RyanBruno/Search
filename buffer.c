#include "buffer.h"

buffer_t buffer_create(size_t s, int c)
{
    buffer_t b;
    struct array* first_array;

    b = array_create(sizeof(struct array), 1);

    /* Setup first array */
    first_array = array_insert(&b);
    *first_array = array_create(s, c);

    return b;
}

void *buffer_insert(buffer_t* b)
{
    for (int i = 0; ; i++) {
        struct array* a;

        a = array_get(b, i);

        if (a == NULL) {
            struct array* first_array;
            struct array new_array;

            first_array = array_get(b, 0);

            printf("a == NULL: \n");
            a = array_insert(b);
            new_array = array_create(first_array->array_s, first_array->array_c);
            *a = new_array;
        }

        if (array_full(a)) {
            continue;
        }
        
        printf("Please dont resize\n");
        return array_insert(a);
    }
}
