#include <stdatomic.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef INT_LL_H
#define INT_LL_H

// An Atomic Integer Linked List Node
typedef struct _integer_linked_list_node_t
{
    atomic_int data;       // Holds the node integer value
    atomic_uintptr_t next; // Holds the next node in the linked list
    atomic_flag deleted;   // Holds if the node is marked for deletion
} node;

// An Atomic Integer Linked List
typedef struct _integer_linked_list_t
{
    atomic_uintptr_t head; // Holds the head of the list
    atomic_int sz;         // Holds the size of the list

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

// Creates a new node
node *create_node(int value);

#endif
