#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include "pthread.h"

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&(list->mutex), NULL); 
    list->root = NULL;              
    list->length = 0;                 
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    if (list->length == 0)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    node_t* aux;
    while (list->root != NULL)
    {
        aux = list->root;
        list->root = list->root->next;
        free(aux);
    }
    list->length = 0;

    
    pthread_mutex_unlock(&(list->mutex));
    pthread_mutex_destroy(&(list->mutex));
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    int length = list->length;
    pthread_mutex_unlock(&(list->mutex));
    return length;
}

// Get node at index
node_t *find_node(int_ll_t *list, int index)
{
    int count = 0;
    if (index < 0)
    {
        index = 0;
    }

    node_t* aux = list->root;
    while (aux != NULL)
    {
        if (count == index || count == list->length -1)
        {
            break;
        }

        aux = aux->next;
        count++;
    }

    return aux;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));
    if (list->length == 0)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    node_t *aux = find_node(list, index);
    *out_value = aux->value;

    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));
    node_t *aux = (node_t*)malloc(sizeof(node_t)); //Inicializando el nodo a insertar
    if (aux == NULL)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }
    if (list->length == 0) //Caso donde la lista esta vacia
    {
        list->root = aux;
    }
    else if (index <= 0) //Caso donde el elemento con el indice a insertar no tiene nodo anterior
    {
        aux->next = list->root;
        list->root = aux;
    }
    else //Casos restantes
    {
        node_t *found = find_node(list, index-1);
        aux->next = found->next;
        found->next = aux;
    }
    
    aux->value = value;
    list->length ++;
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));
    node_t *aux;
    if (list->length == 0) //Caso donde la lista esta vacia
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    else if (index <= 0) //Caso donde el elemento a eliminar es la raiz
    {
        node_t *found = find_node(list, index);
        aux = found;
        list->root = found->next;
    }
    else if (index > list->length-1) //Caso donde el elemento a eliminar sea el final de la lista
    {
        node_t *found = find_node(list, list->length-2);
        aux = found->next;
        found->next = NULL;
    }
    else{ //Caso restante: eliminar un nodo que no esta en los extremos de la lista enlazada
        node_t *found = find_node(list, index-1);
        aux = found->next;
        found->next = aux->next;
    }
    
    *out_value = aux->value;
    free(aux);
    list->length --;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}
