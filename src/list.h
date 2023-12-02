#ifndef INT_LL_H
#define INT_LL_H

#include <pthread.h>

typedef struct node_t
{
    struct node_t *next;
    int val;

} node_t;

// Integer Linked Lists
typedef struct _integer_linked_list_t
{
    int size;
    node_t *head;

    pthread_mutex_t lock;

} int_ll_t;

node_t *create_node(int val);
int_ll_t *create_list();

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
#endif
