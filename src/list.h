#include <semaphore.h>
#include <pthread.h>

#ifndef INT_LL_H
#define INT_LL_H

//Nodo de la lista
typedef struct _integer_linked_list_node_t
{
  int value;//Valor almacenado en la lista
  struct _integer_linked_list_node_t *next;//Próximo nodo de la lista
} int_ll_node_t;

// Integer Linked Lists
typedef struct _integer_linked_list_t
{
  int_ll_node_t *head; //apuntador a la cabeza de la lista
  int size; //tamaño de la lista
  sem_t *sem; //semáforo para limitar el acceso a la estructura de la lista
  //sem_t *sem_empty; //semáforo para evitar acceso de index_list y remove_list cuando la lista esté vacía
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
