#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Biblioteca para manejar hilos y bloqueos

// Init list structure
// Inicializa la lista
int init_list(int_ll_t* list) {
    pthread_mutex_init(&(list->mutex), NULL); // Inicializa el mutex
    list->inicio = NULL;
    list->tamaño = 0;
    return 0; // Éxito
}

// Free list structure
// Libera la memoria utilizada por la lista
int free_list(int_ll_t* list) {
    if (list == NULL) return -1;

    Nodo* actual = list->inicio;
    while (actual != NULL) 
    {
        Nodo* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    free(list);
    // list->inicio = NULL;
    // list->tamaño = 0;

    //pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    pthread_mutex_destroy(&list->mutex); // Destruye el mutex
    return 0; // Éxito
}

// Get list size
// Retorna el tamaño de la lista
int size_list(int_ll_t* list) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista
    int tam = list->tamaño;
    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    return tam;
}

// Get element at index
// Retorna el elemento en un índice especificado
int index_list(int_ll_t* list, int index, int* out_value) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista
    if (list == NULL || list->tamaño == 0)
    {
        pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
        return 1;
    }
    if (index < 0) index = 0;
    else if (index >= list->tamaño) index = list->tamaño - 1;

    Nodo* actual = list->inicio;
    for (int i = 0; i < index; ++i) 
    {
        if (actual != NULL) actual = actual->siguiente;
    }
    *out_value = actual->dato;
    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    return 0; // Éxito
}

// Insert element at index
// Inserta un elemento en un índice especificado
int insert_list(int_ll_t* list, int index, int value) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista
    if (list == NULL)
    {
        pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
        return 1;
    }

    if (index < 0) index = 0;
    else if (index > list->tamaño) index = list->tamaño;

    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->dato = value;

    if (index == 0 || list->inicio == NULL) 
    {
        nuevoNodo->siguiente = list->inicio;
        list->inicio = nuevoNodo;
    } 
    else 
    {
        Nodo* actual = list->inicio;
        for (int i = 0; i < index - 1; ++i) 
        {
            if (actual != NULL) actual = actual->siguiente;
        }
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }

    list->tamaño++;

    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    return 0; // Éxito
}

// Remove element at index
// Elimina un elemento en un índice especificado
int remove_list(int_ll_t* list, int index, int* out_value) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista

    if (list == NULL || list->tamaño == 0)
    {
        pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
        return 1;
    }

    if (index < 0) index = 0;
    else if (index >= list->tamaño) index = list->tamaño - 1;

    Nodo* nodoEliminar;
    if (index == 0) 
    {
        nodoEliminar = list->inicio;
        list->inicio = nodoEliminar->siguiente;
    } 
    else 
    {
        Nodo* actual = list->inicio;
        for (int i = 0; i < index - 1; ++i) 
        {
            actual = actual->siguiente;
        }
        nodoEliminar = actual->siguiente;
        actual->siguiente = nodoEliminar->siguiente;
    }

    *out_value = nodoEliminar->dato;
    free(nodoEliminar);
    list->tamaño--;
    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    return 0; // Éxito
}

