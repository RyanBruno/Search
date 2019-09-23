#include "heap.h"

void heap_create(heap* h_r, int n)
{
    array_create(h_r, sizeof(struct heap_item), n);
}

void heap_insert(heap* h, void* item, float priority)
{
    struct heap_item* ptr;

    ptr = (struct heap_item*) array_insert(h);
    ptr->heap_item = item;
    ptr->heap_priority = priority;

    /* Bubble up */
    int index = h->array_n - 1;
    while (index > 0) {
        int parent_index;
        struct heap_item* parent;
        struct heap_item* child;

        parent_index = (index - 1) / 2;
        parent = array_get(h, parent_index);
        child = array_get(h, index);

        if (child->heap_priority < parent->heap_priority) {
            array_swap_index(h, index, parent_index);
            index = parent_index;
        } else {
            break;
        }
    }
}

void *heap_pop(heap* h)
{
    struct heap_item* item;

    /* Swap first and last item */
    array_swap_index(h, 0, array_size(h) - 1);
    item = array_pop(h);

    if (array_size(h) > 1) {
    /* Bubble down */
    int index = 0;
    while(1) {
        int index_left = (index * 2) + 1;
        int index_right = (index * 2) + 2;

        struct heap_item* parent;
        struct heap_item* left_child;
        struct heap_item* right_child;

        float left_priority = 99999.0;
        float right_priority = 99999.0;

        parent = array_get(h, index);
        left_child = array_get(h, index_left);
        right_child = array_get(h, index_right);

        if (left_child != NULL) {
            left_priority = left_child->heap_priority;
        }

        if (right_child != NULL) {
            right_priority = right_child->heap_priority;
        }

        if (left_priority < right_priority) {
            if (left_priority < parent->heap_priority) {
                array_swap_index(h, index, index_left);
                index = index_left;
            } else {
                break;
            }
        } else {
            if (right_priority < parent->heap_priority) {
                array_swap_index(h, index, index_right);
                index = index_right;
            } else {
                break;
            }
        }
    }
    }

    return item->heap_item;
}

void array_swap_index(struct array* arr, int a, int b)
{
        struct heap_item temp;
        struct heap_item *a_item;
        struct heap_item *b_item;

        a_item = array_get(arr, a);
        b_item = array_get(arr, b);
        
        temp = *a_item;
        *a_item = *b_item;
        *b_item = temp;
}
