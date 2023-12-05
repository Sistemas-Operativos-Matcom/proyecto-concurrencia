#ifndef INT_LL_H
#define INT_LL_H

#include <pthread.h>
#include <stdlib.h>  // Se añade la inclusión de la librería stdlib.h para usar malloc y free

// Estructura de un nodo de la lista enlazada
typedef struct linked_list_node
{
    int value;
    struct linked_list_node *next_node;
} ll_node_t;

// Estructura para la lista enlazada de enteros
typedef struct _integer_linked_list_t
{
    ll_node_t *head;           // Puntero al primer nodo de la lista
    int size;                  // Tamaño actual de la lista
    pthread_mutex_t mutex;     // Mutex para garantizar operaciones atómicas en la lista
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