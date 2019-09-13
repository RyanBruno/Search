/* Ryan Bruno */
#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <string.h>
#include "array.h"

typedef struct array stack;

void stack_push(struct array* s, void *i);
void *stack_pop(struct array* s);

#endif
