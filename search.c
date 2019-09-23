/* Ryan Bruno */
#include <stdio.h>
#include <ctype.h>
#include "array.h"
#include "stream.h"
#include "heap.h"

struct path {
    struct node* node;
    struct path* next;
    int cost;
};

struct edge {
    int id;
    float weight;
};

struct node {
    int id;
    struct array edges; // Array of edges
};

void parse_heuristics(void** cb_data);
struct node* find_node_by_id(struct array* node_array, int id);
int isnumber(const char* s);
int is_id_in_path(struct path* p, int id);
void print_path(struct path* p, float sum);

/* The a star search */
void a_star_search(struct array* node_array,
        struct array* heuristics_array,
        int start, int end)
{
    heap h;

    h = heap_create(100);
    /* Add start node to adt */
    {
        struct path* p;

        p = malloc(sizeof(struct path));
        p->node = find_node_by_id(node_array, start);
        p->next = NULL;
        p->cost = 0;

        heap_insert(&h, p, 0);
    }

    int t = 0;
    while(1) {
        struct path* p;
        struct array* edges;
        
        if (t++ > 3) return;
        /* Get next node */
        p = (struct path*) heap_pop(&h);
        edges = &(p->node->edges);
        //printf("Node %d: \n", p->node->id);

        /* Check if we are at the end */
        if (p->node->id == end) {
            print_path(p->next, 0);
            printf("%d\n", p->node->id);
            break;
        }

        /* Add all the edges to heap */
        for (int i = 0; i < array_size(edges); i++) {
            struct edge* e;
            struct node* node;
            struct path* np;
            struct array* to_array;
            int from_id = end;
            float *heuristic;

            /* Get edge */
            e = (struct edge*) array_get(edges, i);
            //printf(" = Edge %d ->%d:%f \n", p->node->id, e->id, e->weight);


            /* Check if node is already visited */
            if (is_id_in_path(p, e->id) == 0) {
                //printf("Aleady in path %d\n", e->id);
                continue;
            }

            /* Get lowest id for heuristic lookup */
            from_id = end;
            if (e->id < end) {
                from_id = e->id;
            }

            if (e->id == end) {
                heuristic = malloc(sizeof(float));
                *heuristic = 0;
            } else {
                /* Find heuristic */
                to_array = (struct array*) array_get(heuristics_array, from_id - 1);
                heuristic = (float*) array_get(to_array, abs(e->id - end) - 1);
                //printf("Heuristic: %d -> %d, %f\n", e->id, end, *heuristic);
            }

            /* Add node with path to adt */
            node = find_node_by_id(node_array, e->id);

            //printf("Adding %d to heap\n", node->id);
            np = malloc(sizeof(struct path));
            np->node = node;
            np->next = p;
            np->cost = e->weight;
            //printf("Combnded he %f\n", np->cost + *heuristic);
            heap_insert(&h, np, np->cost + *heuristic);
        }
    }
}

int main()
{
    char* buffer;
    int start = 100;
    int end = 1;
    FILE *weight_file_ptr;
    FILE *heuristics_file_ptr;
    struct array node_array;
    void* cb_data[2];

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
    node_array = array_create(sizeof(struct node), 200);

    while(feof(weight_file_ptr) == 0){
        struct node* n;
        int id;
        struct edge* edge;

        /* Read first number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        id = atoi(buffer);
        n = find_node_by_id(&node_array, id);

        if (n == NULL) {
            /* Initalize node */
            n = (struct node*) array_insert(&node_array);
            n->edges = array_create(sizeof(struct edge), 10);
            n->id = id;
        }

        
        /* Read second number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Put edge in node's edge array with weight */
        edge = array_insert(&(n->edges));
        edge->id = atoi(buffer);
        edge->weight = 100000; // Temp value

        /* Read third number */
        stream_first(weight_file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(weight_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        edge->weight = atof(buffer);
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

    struct array heuristics_array;
    heuristics_array = array_create(sizeof(struct array), 200);

    while(feof(heuristics_file_ptr) == 0){
        struct array* to_array;
        struct array temp;

        /* Read first number */
        stream_first(heuristics_file_ptr, buffer, ',', '\n');

        if (isnumber(buffer) == 0) {
            stream_each(heuristics_file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Get int */
        //from_id = atof(buffer);

        /* Create array */
        to_array = (struct array*) array_insert(&heuristics_array);
        temp = array_create(sizeof(float), 50);
        *to_array = temp;

        cb_data[0] = buffer;
        cb_data[1] = to_array;
        /* Parse rest of row */
        stream_each(heuristics_file_ptr, buffer, ',', '\n', parse_heuristics, &cb_data);
        /* Get Last line fix later */
        parse_heuristics(&cb_data);
    }

    printf("Start node: ");
    scanf("%d", &start);
    printf("End node: ");
    scanf("%d", &end);

    a_star_search(&node_array, &heuristics_array, start, end);
}

void parse_heuristics(void** cb_data)
{
    const char* buffer = (const char*) cb_data[0];
    struct array* to_array = (struct array*) cb_data[1];
    float from_id;
    float *temp_id;

    if (isnumber(buffer) == 0) {
        return;
    }

    /* Get float */
    from_id = atof(buffer);

    if (from_id == 0) {
        return;
    }

    /* Set number to from_id */
    temp_id = (float*) array_insert(to_array);
    *temp_id = from_id;
}

int is_id_in_path(struct path* p, int id)
{
    while (p != NULL) {
        if (id == p->node->id) {
            return 0;
        }
        p = p->next;
    }
    return 1;
}

struct node*
find_node_by_id(struct array* node_array, int id)
{

    for (int i = 0; i < array_size(node_array); i++) {
        struct node* temp;
        
        temp = (struct node*) array_get(node_array, i);
        if (temp->id == id) {
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

void print_path(struct path* p, float sum)
{
    if (p == NULL) {
        printf("[ %f ] ", sum);
        return;
    }

    print_path(p->next, sum + p->cost);
    printf("%d -> ", p->node->id);
}
