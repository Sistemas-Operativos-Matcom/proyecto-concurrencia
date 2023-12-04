#include <pthread.h>

typedef struct int_linked_list_node_t
{
    int val;
    struct int_linked_list_node_t *next;
} int_ll_node_t;

// Integer Linked Lists
typedef struct _integer_linked_list_t
{
    int size;
    int_ll_node_t *first;
    pthread_mutex_t mutex;

} int_ll_t;

// Init list structure
int init_list(int_ll_t *list);

// Free list structure
int free_list(int_ll_t *list);

// Get list size
int size_list(int_ll_t *list);

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value);

// Insert element at index
int insert_list(int_ll_t *list, int index, int value);

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value);
