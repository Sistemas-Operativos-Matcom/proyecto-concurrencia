#include "list.h"
#include <stdlib.h>
#include <pthread.h>

#define ERROR 1
// Init list structure
int init_list(int_ll_t *list)
{
    list->cantidad = 0;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    //se hace free a cada casilla de la lista(cada una es un puntero)
    while (list->cantidad > 0)
    {
        celda* c = list->inicio;
        list->inicio = list->inicio->contiguo;
        free(c);
        list->cantidad = list->cantidad - 1;
    }
    //se hace free al puntero de la lista
    free(list);
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    int size = list->cantidad;
    pthread_mutex_unlock(&list->lock);
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    
    if (index >= list->cantidad)
    {
        index = list->cantidad - 1;
    }
    if (index < 0)
    {
        index = 0;
    }
    celda* actual;
    if(list->cantidad > 0)
    {
        actual = list->inicio;
        int i = 0;
        //se busca la casilla con ese indice
        while (i < index)
        {
            actual = actual->contiguo;
            i++;
        }
        *out_value = actual->valor;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
//se busca la casilla con un indice menor y se hace q este apunte a la nueva
// casilla creada y esta apuntara a la casilla que apuntaba la casilla con
//indice anterior
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    if (index >= list->cantidad)
        index = list->cantidad - 1;

    if (index < 0)
        index = 0;

    if(index == 0)
    {
        celda* incertar = (celda*)malloc(sizeof(celda));
        incertar->valor = value;
        incertar->contiguo = list->inicio;
        list->inicio = incertar;
    }
    if(index > 0)
    {
        celda* incertar = (celda*)malloc(sizeof(celda));
        incertar->valor = value;
        celda* actual = list->inicio;
        int i = 0;
        while (i < index - 1)
        {
            actual = actual->contiguo;
            i++;
        }
        incertar->contiguo = actual->contiguo;
        actual->contiguo = incertar;
    }
    list->cantidad = list->cantidad + 1;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
//busca la casilla con indice anterior y se hace que esta apunte a la
//casilla siguiente a la casilla con el indice especificado
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock); 
    if(list->cantidad == 0)
    {
       pthread_mutex_unlock(&list->lock);
        return ERROR; 
    }
    int actual;
    if (index >= list->cantidad)
    {
        index = list->cantidad - 1;
    }
    if (index < 0)
    {
        index = 0;
    }
    if(index == 0)
    {
        actual = list->inicio->valor;
        list->inicio = list->inicio->contiguo;
    }
    if(index > 0)
    {
        celda* c = list->inicio;
        int i = 0;
        while (i < index - 1)
        {
            c = c->contiguo;
            i++;
        }
        actual = c->contiguo->valor;
        c->contiguo = c->contiguo->contiguo;
    }
    *out_value = actual;
    list->cantidad = list->cantidad - 1;
    pthread_mutex_unlock(&list->lock);
    return 0;
}
