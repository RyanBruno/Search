/* Ryan Bruno */
#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct array {
    int array_c;
    int array_n;
    int array_s;
    char *array_data;
};

struct array array_create(size_t s, int c);
void *array_insert(struct array* a);
void *array_get(struct array* a, int n);
void *array_get_end(struct array* a);
void *array_pop(struct array* a);
int array_size(struct array* a);


#endif
