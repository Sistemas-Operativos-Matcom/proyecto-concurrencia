#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#ifndef INT_LL_H
#define INT_LL_H
// Integer Linked Lists
 
#include "stdio.h"

typedef struct node
{
    int index;
    int value;
    struct node *next;
} Node;  

/*Node *newNode(int index, int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    node->index = index;
    node->next = NULL;
    return node;
}*/
typedef struct _integer_linked_list_t
{
    Node *root;
    int size;
    Node *last;
    pthread_mutex_t lock;
} int_ll_t;

/*int_ll_t *newList()
{
    int_ll_t *list = (int_ll_t *)malloc(sizeof(int_ll_t));
    list->root = NULL;
    list->size = 0;

}*/




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
