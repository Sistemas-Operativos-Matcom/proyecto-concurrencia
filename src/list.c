#include "list.h"
#include <pthread.h> // Biblioteca para manejar hilos y bloqueos

// Init list structure
// Inicializa la lista
int init_list(int_ll_t* list) {
    list->inicio = NULL;
    list->tamaño = 0;
    pthread_mutex_init(&list->mutex, NULL); // Inicializa el mutex
    return 1; // Éxito
}

// Free list structure
// Libera la memoria utilizada por la lista
int free_list(int_ll_t* list) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista

    Nodo* actual = list->inicio;
    while (actual != NULL) 
    {
        Nodo* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    list->inicio = NULL;
    list->tamaño = 0;

    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    pthread_mutex_destroy(&list->mutex); // Destruye el mutex
    return 1; // Éxito
}

// Get list size
// Retorna el tamaño de la lista
int size_list(int_ll_t* list) {
    return list->tamaño;
}

// Get element at index
// Retorna el elemento en un índice especificado
int index_list(int_ll_t* list, int index, int* out_value) {
    if (index < 0) index = 0;
    else if (index >= list->tamaño) index = list->tamaño - 1;

    Nodo* actual = list->inicio;
    for (int i = 0; i < index; ++i) 
    {
        actual = actual->siguiente;
    }
    *out_value = actual->dato;
    return 1; // Éxito
}

// Insert element at index
// Inserta un elemento en un índice especificado
int insert_list(int_ll_t* list, int index, int value) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista

    if (index < 0) index = 0;
    else if (index > list->tamaño) index = list->tamaño;

    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->dato = value;

    if (index == 0) 
    {
        nuevoNodo->siguiente = list->inicio;
        list->inicio = nuevoNodo;
    } 
    else 
    {
        Nodo* actual = list->inicio;
        for (int i = 0; i < index - 1; ++i) 
        {
            actual = actual->siguiente;
        }
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }

    list->tamaño++;

    pthread_mutex_unlock(&list->mutex); // Desbloquea el acceso a la lista
    return 1; // Éxito
}

// Remove element at index
// Elimina un elemento en un índice especificado
int remove_list(int_ll_t* list, int index, int* out_value) {
    pthread_mutex_lock(&list->mutex); // Bloquea el acceso a la lista

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
    return 1; // Éxito
}

// int main() {
//     // Ejemplo de uso
//     int_ll_t lista;
//     init_list(&lista);

//     insert_list(&lista, 0, 10);
//     insert_list(&lista, 1, 20);
//     insert_list(&lista, 1, 15);

//     int value;
//     index_list(&lista, 1, &value);
//     printf("Elemento en el índice 1: %d\n", value);

//     remove_list(&lista, 1, &value);
//     printf("Elemento eliminado: %d\n", value);

//     free_list(&lista);
//     return 0;
// }
