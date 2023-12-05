#ifndef INT_LL_H
#define INT_LL_H
#include <pthread.h>
typedef struct Element
// estructura de los elementos de la linked list
{
    int value; //valor del elemento
    struct Element *next; //puntero al siguiente elemento
}Element_t;

// Integer Linked Lists
typedef struct _integer_linked_list_t
{
    int size; //cantidad de elementos almacenados
    Element_t *First; //puntero al cabezal de la lista
    pthread_mutex_t lock; //estructura de bloqueo
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
