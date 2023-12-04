#ifndef INT_LL_H
#define INT_LL_H

#include <pthread.h>
#include <stdlib.h>

typedef struct _int_list_t
{
    node* root;
    int size;
    pthread_mutex_t lock;
} linkedlist_t;

typedef struct node {
    int value;
    struct node* next;
} node;


// Init list structure
int init_list(linkedlist_t *list);

// Free list structure
int free_list(linkedlist_t *list);

// Get list size
int size_list(linkedlist_t *list);

// Get element at index
int index_list(linkedlist_t *list, int index, int *out_value);

// Insert element at index
int insert_list(linkedlist_t *list, int index, int value);

// Remove element at index
int remove_list(linkedlist_t *list, int index, int *out_value);
#endif