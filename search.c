/* Ryan Bruno */
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include "array.h"
#include "stream.h"
#include "heap.h"

struct path {
    struct node* path_node;
    struct path* path_next;
    float path_cost;
};

struct edge {
    int edge_id;
    float edge_weight;
};

struct node {
    int node_id;
    struct array node_edges;
};

void parse_heuristics(void** cb_data);
struct node* find_node_by_id(struct array* node_array, int id);
float get_heuristic(struct array* heuristics_array, int a, int b);
int isnumber(const char* s);
int is_id_in_path(struct path* p, int id);
void print_path(struct path* p);

/* The a_star algorithm */
float a_star_search(struct array* node_array,
        struct array* heuristics_array, heap* h,
        int start, int end, float id_max_cost)
{
    float next_id_max_cost;
    next_id_max_cost = FLT_MAX;

    /* Add start node to heap */
    {
        struct path* p;

        p = malloc(sizeof(struct path));
        p->path_node = find_node_by_id(node_array, start);
        p->path_next = NULL;
        p->path_cost = 0;

        heap_insert(h, p, 0);
    }

    while(array_size(h) > 0) {
        struct path* p;
        struct node* n;
        
        /* Get next node with lowest cost */
        p = (struct path*) heap_pop(h);
        n = p->path_node;

        /* Check if we are at the end */
        if (n->node_id == end) {
            printf("[ %.2f ] ", p->path_cost);
            print_path(p->path_next);
            printf("%d\n", n->node_id);
            return -1;
        }

        /* Add all the edges to heap */
        for (int i = 0; i < array_size(&(n->node_edges)); i++) {
            struct edge* e;
            struct path* np;
            float cost;
            float heuristic;

            /* Get edge */
            e = array_get(&(n->node_edges), i);

            /* Check if node is already visited */
            if (is_id_in_path(p, e->edge_id) == 0) {
                continue;
            }

            /* Check if over id_max_cost */
            cost = p->path_cost + e->edge_weight;
            if (cost > id_max_cost) {
                if (cost < next_id_max_cost)
                    next_id_max_cost = cost;
                continue;
            }

            /* Get heuristic */
            heuristic = get_heuristic(heuristics_array, e->edge_id, end);

            /* Add edge to heap */
            np = malloc(sizeof(struct path));
            np->path_node = find_node_by_id(node_array, e->edge_id);
            np->path_next = p;
            np->path_cost = cost;
            heap_insert(h, np, cost + heuristic);
        }
    }

    return next_id_max_cost;
}

int main()
{
    char* buffer;
    int start = 100;
    int end = 1;
    FILE *weight_file_ptr;
    FILE *heuristics_file_ptr;
    struct array node_array;
    struct array heuristics_array;
    void* cb_data[2];
    heap h;

    /* Create buffer for stream input */
    buffer = malloc(100);

    printf("Please enter edge weight file name and extension: ");
    scanf("%s", buffer);

    weight_file_ptr = fopen(buffer, "r");

    if (weight_file_ptr == NULL) {
        printf("Trying EdgeWeights.csv\n");

        strcpy(buffer, "EdgeWeights.csv");
        weight_file_ptr = fopen(buffer, "r");

        if (weight_file_ptr == NULL) {
            printf("Could not open file\n");
            return 1;
        }
    }

    /* Create the node array */
    array_create(&node_array, sizeof(struct node), 200);

    while(feof(weight_file_ptr) == 0){
        int id;
        struct node* n;
        struct edge* e;

        /* Read first number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Get node */
        id = atoi(buffer);
        n = find_node_by_id(&node_array, id);

        /* Initalize node if not already done */
        if (n == NULL) {
            n = (struct node*) array_insert(&node_array);
            array_create(&(n->node_edges), sizeof(struct edge), 10);
            n->node_id = id;
        }

        
        /* Read second number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Put edge in node's edge array with weight */
        e = array_insert(&(n->node_edges));
        e->edge_id = atoi(buffer);
        e->edge_weight = FLT_MAX; // Temp value

        /* Read third number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        e->edge_weight = atof(buffer);
    }

    printf("Please enter heuristics file name and extension: ");
    scanf("%s", buffer);
    heuristics_file_ptr = fopen(buffer, "r");

    if (heuristics_file_ptr == NULL) {
        printf("Trying minCosts.csv\n");

        strcpy(buffer, "minCosts.csv");
        heuristics_file_ptr = fopen(buffer, "r");

        if (heuristics_file_ptr == NULL) {
            printf("Could not open file\n");
            return 1;
        }
    }

    /* Create the heuristics array */
    array_create(&heuristics_array, sizeof(struct array), 200);

    while(feof(heuristics_file_ptr) == 0){
        struct array* to_array;

        /* Read first number */
        stream_first(heuristics_file_ptr, buffer, ',', '\n');

        if (isnumber(buffer) == 0) {
            stream_each(heuristics_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Create array */
        to_array = (struct array*) array_insert(&heuristics_array);
        array_create(to_array, sizeof(float), 50);

        /* Set first value to zero */
        *(float*) array_insert(to_array) = 0;

        /* Setup callback data */
        cb_data[0] = buffer;
        cb_data[1] = to_array;
        /* Parse rest of row with parse_heurisitics function */
        stream_each(heuristics_file_ptr, buffer, ',', '\n', parse_heuristics, &cb_data);
        /* Read last value (temp work-around) */
        parse_heuristics((void**) &cb_data);
    }

    printf("Start node: ");
    scanf("%d", &start);
    printf("End node: ");
    scanf("%d", &end);

    /* Create a heap for a_star */
    heap_create(&h, 100);

    /* Run the a_star algorithm */
    printf("A* minimum cost path\n");
    a_star_search(&node_array, &heuristics_array, &h, start, end, FLT_MAX);

    /* Run the IDa_star algorithm */
    printf("IDA* minimum cost path\n");
    for (float id_max_cost = 0; id_max_cost != -1;) {
        array_reset(&h);
        id_max_cost = a_star_search(&node_array, &heuristics_array, &h, start, end, id_max_cost);
    }
}

void parse_heuristics(void** cb_data)
{
    const char* buffer = (const char*) cb_data[0];
    struct array* to_array = (struct array*) cb_data[1];
    float heuristic;

    if (isnumber(buffer) == 0) {
        return;
    }

    /* Get float */
    heuristic = atof(buffer);

    /* Skip all zeros */
    if (heuristic == 0) {
        return;
    }

    /* Add heuristic to to_array */
    *(float*) array_insert(to_array) = heuristic;
}

float get_heuristic(struct array* heuristics_array, int a, int b)
{
    struct array* to_array;
    float *heuristic;

    /* Make a the lowest number */
    if (b < a) {
        int temp = a;
        a = b;
        b = temp;
    }

    /* Find heuristic */
    to_array = (struct array*) array_get(heuristics_array, a - 1);
    heuristic = (float*) array_get(to_array, abs(a - b));

    return *heuristic;
}

int is_id_in_path(struct path* p, int id)
{
    while (p != NULL) {
        if (id == p->path_node->node_id) {
            return 0;
        }
        p = p->path_next;
    }
    return 1;
}

struct node*
find_node_by_id(struct array* node_array, int id)
{
    for (int i = 0; i < array_size(node_array); i++) {
        struct node* temp;
        
        temp = (struct node*) array_get(node_array, i);
        if (temp->node_id == id) {
            return temp;
        }
    }
    return NULL;
}

int isnumber(const char* s) {
    if (strlen(s) == 0) return 0;

    for (int i = 0; i < strlen(s); i++) {
        if (isdigit(s[i]) == 0 && s[i] != '.') {
            return 0;
        }
    }
    return 1;
}

void print_path(struct path* p)
{
    if (p == NULL)
        return;

    print_path(p->path_next);
    printf("%d -> ", p->path_node->node_id);
}
