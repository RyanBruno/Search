#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"

typedef struct array buffer_t;

buffer_t buffer_create(size_t s, int c);
void *buffer_insert(buffer_t* b);

#endif
