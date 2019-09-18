/* Ryan Bruno */
#include <stdio.h>
#include <ctype.h>
#include "array.h"
#include "stream.h"
#include "stack.h"
#include "queue.h"

struct adt_vfuncts {
    void (*in)(void* adt, void *i);
    void *(*out)(void* adt);
};

struct adt_vfuncts stack_vfuncts = {
    stack_push,
    stack_pop
};

struct adt_vfuncts queue_vfuncts = {
    queue_enqueue,
    queue_dequeue
};

struct path {
    struct node* node;
    struct path* next;
};

struct node {
    int id;
    struct array edges; // Array of ints
};

void parse_edges(void** cb_data);
struct node* find_node_by_id(struct array* node_array, int id);
int isnumber(const char* s);
int is_id_in_path(struct path* p, int id);
void print_path(struct path* p);

/* The BF and DF search */
/* Parameter adt is either a stack or queue */
void search(struct array* node_array,
        int start, int end,
        void* adt, struct adt_vfuncts* vfuncts)
{
    /* Add start node to adt */
    {
        struct path* p;

        p = malloc(sizeof(struct path));
        p->node = find_node_by_id(node_array, start);
        p->next = NULL;

        vfuncts->in(adt, p);
    }

    while(1) {
        struct path* p;
        struct array* edges;
        
        /* Get next node */
        p = (struct path*) vfuncts->out(adt);
        edges = &(p->node->edges);

        /* Check if we are at the end */
        if (p->node->id == end) {
            print_path(p->next);
            printf("%d", p->node->id);
            break;
        }

        /* Add all the edges to adt */
        for (int i = 0; i < array_size(edges); i++) {
            int* id;
            struct node* node;
            struct path* np;

            /* Get edge */
            id = (int*) array_get(edges, i);

            /* Check if node is already visited */
            if (is_id_in_path(p, *id) == 0) {
                continue;
            }

            /* Add node with path to adt */
            node = find_node_by_id(node_array, *id);
            np = malloc(sizeof(struct path));
            np->node = node;
            np->next = p;
            vfuncts->in(adt, np);
        }
    }
}

int main()
{
    char* buffer;
    int start = 100;
    int end = 1;
    FILE *file_ptr;
    struct array node_array;
    void* cb_data[2];

    /* Create buffer for stream input */
    buffer = malloc(100);

    printf("Please enter the file name and extension: ");
    scanf("%s", buffer);

    file_ptr = fopen(buffer, "r");

    if (file_ptr == NULL) {
        printf("Could not open file\n");
        printf("Trying BFS_DFS.csv\n");

        strcpy(buffer, "BFS_DFS.csv");
        file_ptr = fopen(buffer, "r");

        if (file_ptr == NULL) {
            printf("Could not open file\n");
            return 1;
        }
    }

    printf("%s\n", buffer);
    printf("Start node: ");
    scanf("%d", &start);
    printf("End node: ");
    scanf("%d", &end);

    /* Create the node array */
    node_array = array_create(sizeof(struct node), 200);

    while(feof(file_ptr) == 0){
        struct node* n;

        /* Read first number */
        stream_first(file_ptr, buffer, ',', '\n'); // check ret

        if (isnumber(buffer) == 0) {
            stream_each(file_ptr, buffer, ',', '\n', NULL, NULL);
            continue;
        }

        /* Initalize node */
        n = (struct node*) array_insert(&node_array);
        n->edges = array_create(sizeof(int), 10);
        n->id = atoi(buffer);

        // cb_data = [ cosnt char* buffer, node* n ]
        cb_data[0] = (void*) buffer;
        cb_data[1] = (void*) n;
        stream_each(file_ptr, buffer, ',', '\n', parse_edges, &cb_data);
    }

    /* Create stack */
    stack s;
    s = array_create(sizeof(struct path*), 200);

    /* Run DFS */
    printf("Depth-first traversal\n");
    search(&node_array, start, end, &s, &stack_vfuncts);
    printf("\n");

    /* Create queue */
    struct queue q;
    q = queue_create();

    /* Run BFS */
    printf("Breadth-first traversal\n");
    search(&node_array, start, end, &q, &queue_vfuncts);
    printf("\n");
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

void parse_edges(void** cb_data)
{
    const char* buffer = (const char*) cb_data[0];
    struct node* n = (struct node*) cb_data[1];
    int *id;
    
    if (isnumber(buffer) != 0) {
        id = array_insert(&(n->edges));
        *id = atoi(buffer);
    }
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
        if (isdigit(s[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

void print_path(struct path* p)
{
    if (p == NULL)
        return;

    print_path(p->next);
    printf("%d -> ", p->node->id);
}
