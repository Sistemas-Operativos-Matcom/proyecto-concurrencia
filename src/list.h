#ifndef INT_LL_H
#define INT_LL_H
#include <pthread.h>
// Integer Linked Lists

//cada posicion/nodo de la linked list
typedef struct pos{
    //valor almacenado en la posicion
    int value;
    //el nodo siguiente
    struct pos *next;
    //el nodo previo
    struct pos *prev;
} pos_t;

//lista de nodos
typedef struct _integer_linked_list_t
{
    //el primer nodo
    pos_t *head;
    //el size de la lista
    int size;
    pthread_mutex_t lock;
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
#endif
