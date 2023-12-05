#include "list.h"
#include <pthread.h>
#include <stdlib.h>

int init_list(int_ll_t *list)
{
    // Inicializar una lista vacia
    list->size = 0;
    list->First = NULL;

    // Inicializar el bloqueo para esa lista
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    // itera por cada elemento de la lista 
    // libera el espacio correspondiente al primer elemento de la lista
    while (list->size != 0)
    {
        Element_t *temp = list->First;
        list->First = list->First->next;
        list->size--;
        free(temp);
    }
    // devuelve la lista a 0
    list->size = 0;
    pthread_mutex_unlock(&list->lock);  
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    // si la lista es vacia
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }


    if (index <= 0)
    // si es el primer elemento
    {
        *out_value = list->First->value;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    if (index >= list->size)
    {
        index = list->size - 1;
    }
    int count = 0;
    Element_t *curr = list->First;
    while (count < index)
    // itera hasta encontrar el indice deseado
    {
        curr = curr->next;
        count ++;
    }
    *out_value = curr->value;
    pthread_mutex_unlock(&list->lock); 
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    // si la lista era vacia el elemento a añadir es el cabezal de la lista
    {
        Element_t *new = malloc(sizeof(Element_t));
        new->value = value;
        list->First = new;
        list->size = 1;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }

    if (index <= 0)
    // añadir en la primera posicion
    {
        Element_t *new = malloc(sizeof(Element_t));
        new->value = value;
        new->next = list->First;
        list->First = new;
        list->size++;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    Element_t *curr = list->First;
    if (index > list->size - 1)
    {
        index = list->size - 1;
    }
    int count = 0;
    while (count < index -1)
    // itera hasta q curr es el nodo anterior al que se desea añadir
    {
        curr = curr->next;
        count ++;
    }
    Element_t *temp = curr->next;
    Element_t *new = malloc(sizeof(Element_t));

    // el siguiente a curr es el nuevo nodo y el siguiente a este es el que 
    // anteriormente era el siguiente a curr

    curr->next = new;
    new->value = value;
    new->next = temp;
    list->size ++;
    pthread_mutex_unlock(&list->lock); 
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    // si la lista era vacia no tiene sentido eliminar
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    if (list->size == 1)
    // si la lista solo contenia un elemento este es el que se ha de eliminar
    {
        *out_value = list->First->value;
        list->size = 0;
        Element_t *temp = list->First;
        list->First = NULL;
        free(temp);
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    if (index <= 0)
    // si el elemento a eliminar es el primero
    {
        *out_value = list->First->value;
        list->size = list->size -1;
        Element_t *temp = list->First;
        list->First = list->First->next;
        free(temp);
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    Element_t* curr = list->First;
    if (index > list->size - 1)
    {
        index = list->size - 1;
    }
    int count = 0;
    while (count < index -1)
    // itera hasta que curr es el elemento anterior al que debe ser eliminado
    {
        curr = curr->next;
        count ++;
    }
    Element_t *temp = curr->next;
    *out_value = temp->value;

    // el siguiente a curr sera el que era el siguiente del elemento a eliminar

    curr->next = temp->next;
    list->size --;
    free(temp);
    pthread_mutex_unlock(&list->lock); 
    return 0;
}
