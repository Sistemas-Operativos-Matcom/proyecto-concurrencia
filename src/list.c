#include "list.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// Ajustar el índice
int adj_index(int index, int size)
{
    if (index < 0) 
        index = 0;
    else if (index >= size)
        index = size - 1;
    return index;
}

// Inicializar la estructura de la lista
int init_list(int_ll_t *list)
{
    list->head = NULL; // Inicializar la cabeza de la lista como NULL
    list->size = 0; 
    pthread_mutex_init(&list->lock, NULL); // Inicializar el mutex
    return 0;
}

// Liberar la estructura de la lista
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    
    node_t *current = list->head; 
    node_t *next;
    
    while (current != NULL)
    { 
        next = current->next; 
        free(current); 
        current = next;
        
    }

    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    pthread_mutex_destroy(&list->lock); // Destruir el mutex
    return 0;
}

// Obtener el tamaño de la lista
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    int size = list->size; 
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return size;
}

// Obtener el elemento en el índice
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
   
    node_t *current = list->head;

    if (current == NULL) // Si el nodo actual es NULL, la lista está vacía
    { 
        pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
        return 1;
    }

    index = adj_index(index, list->size);  
    
    for (int i = 0; i < index; ++i)
    { 
        current = current->next; // El nodo actual ahora es el siguiente
    }

    *out_value = current->value; // El valor de salida es el valor del nodo actual
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}

// Insertar el elemento en el índice
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex

    index = adj_index(index, list->size);

    node_t *new_node = malloc(sizeof(node_t)); // Crear un nuevo nodo
    new_node->value = value;
    
    if (index <= 0)
    { 
        new_node->next = list->head; // El siguiente del nuevo nodo es la cabeza de la lista
        list->head = new_node;
    } 
    else 
    {
        node_t *current = list->head;
        
        for (int i = 0; i < index - 1; ++i)
            current = current->next;

        new_node->next = current->next; // El siguiente del nuevo nodo es el siguiente del nodo actual
        current->next = new_node;
    }

    list->size++;
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}

// Eliminar el elemento en el índice
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    
    index = adj_index(index, list->size);
    node_t *current = list->head; // Nodo actual

    if (current == NULL) // Si el nodo actual es NULL, la lista está vacía
    { 
        pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
        return 1; // No se puede eliminar nada de una lista vacía
    }

    if (index == 0)
    {        
        list->head = current->next; // La cabeza de la lista ahora es el siguiente del nodo actual
        *out_value = current->value;
        free(current); 
    } 
    else 
    {
        node_t *previous = NULL; 
        for (int i = 0; i < index; i++)
        { 
            previous = current; // El nodo anterior ahora es el nodo actual
            current = current->next;
        }
        previous->next = current->next; // El siguiente del nodo anterior ahora es el siguiente del nodo actual
        *out_value = current->value;
        free(current); 
    }

    list->size--; 
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}
