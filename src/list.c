#include "list.h"
#include <stdlib.h>
#include <pthread.h>

// Inicializar la estructura de la lista
int init_list(int_ll_t *list)
{
    list->head = NULL; // Inicializar la cabeza de la lista como NULL
    list->size = 0; // Inicializar el tamaño de la lista como 0
    pthread_mutex_init(&list->lock, NULL); // Inicializar el mutex
    return 0;
}

// Liberar la estructura de la lista
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    
    node_t *current = list->head; // Nodo actual
    node_t *next; // Nodo siguiente
    
    while (current != NULL) // Mientras el nodo actual no sea NULL
    { 
        next = current->next; // El nodo siguiente es el siguiente del actual
        free(current); // Liberar el nodo actual
        current = next; // El nodo actual ahora es el siguiente
        
    }

    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    pthread_mutex_destroy(&list->lock); // Destruir el mutex
    return 0;
}

// Obtener el tamaño de la lista
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    int size = list->size; // Tamaño de la lista
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return size;
}

// Obtener el elemento en el índice
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
   
    // Ajustar el índice
    if (index < 0) 
        index = 0;
    else if (index >= list->size)
        index = list->size - 1;

    node_t *current = list->head; // Nodo actual
    
    for (int i = 0; i < index && current != NULL; i++) // Recorrer hasta el índice
    { 
        current = current->next; // El nodo actual ahora es el siguiente
    }

    if (current == NULL) // Si el nodo actual es NULL, el índice está fuera de los límites
    { 
        pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
        return -1; // Índice fuera de los límites
    }

    *out_value = current->value; // El valor de salida es el valor del nodo actual
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}

// Insertar el elemento en el índice
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex

    node_t *new_node = malloc(sizeof(node_t)); // Crear un nuevo nodo
    new_node->value = value; // Asignar el valor al nuevo nodo
    
    if (index == 0) // Si el índice es 0
    { 
        new_node->next = list->head; // El siguiente del nuevo nodo es la cabeza de la lista
        list->head = new_node; // La cabeza de la lista ahora es el nuevo nodo
    } 
    else 
    {
        node_t *current = list->head; // Nodo actual
        for (int i = 0; i < index - 1 && current != NULL; i++) // Recorrer hasta el índice - 1
        { 
            current = current->next; // El nodo actual ahora es el siguiente
        }
        if (current == NULL) // Si el nodo actual es NULL, el índice está fuera de los límites
        { 
            free(new_node); // Liberar el nuevo nodo
            pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
            return -1; // Índice fuera de los límites
        }
        new_node->next = current->next; // El siguiente del nuevo nodo es el siguiente del nodo actual
        current->next = new_node; // El siguiente del nodo actual ahora es el nuevo nodo
    }

    list->size++; // Incrementar el tamaño de la lista
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}

// Eliminar el elemento en el índice
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock); // Bloquear el mutex
    
    // Ajustar el índice
    if (index < 0) 
        index = 0;
    else if (index >= list->size)
        index = list->size - 1;

    node_t *current = list->head; // Nodo actual
    
    if (index == 0)  // Si el índice es 0
    {
        if (current == NULL) // Si el nodo actual es NULL, el índice está fuera de los límites
        { 
            pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
            return -1; // Índice fuera de los límites
        }
        list->head = current->next; // La cabeza de la lista ahora es el siguiente del nodo actual
        *out_value = current->value; // El valor de salida es el valor del nodo actual
        free(current); // Liberar el nodo actual
    } 
    else 
    {
        node_t *previous = NULL; // Nodo anterior
        for (int i = 0; i < index && current != NULL; i++) // Recorrer hasta el índice
        { 
            previous = current; // El nodo anterior ahora es el nodo actual
            current = current->next; // El nodo actual ahora es el siguiente
        }
        if (current == NULL) // Si el nodo actual es NULL, el índice está fuera de los límites
        { 
            pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
            return -1; // Índice fuera de los límites
        }
        previous->next = current->next; // El siguiente del nodo anterior ahora es el siguiente del nodo actual
        *out_value = current->value; // El valor de salida es el valor del nodo actual
        free(current); // Liberar el nodo actual
    }

    list->size--; // Decrementar el tamaño de la lista
    pthread_mutex_unlock(&list->lock); // Desbloquear el mutex
    return 0;
}

// ----------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------