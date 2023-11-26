#include "list.h"
#include <stdlib.h>
#include <pthread.h>


// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    
    pthread_mutex_init(&list->lock, NULL);  // Inicializar el bloqueo
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);  // Bloquear antes de manipular la lista

    while (list->head != NULL) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    list->size = 0;

    pthread_mutex_unlock(&list->lock);  // Desbloquear después de manipular la lista
    pthread_mutex_destroy(&list->lock);  // Destruir el bloqueo

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);  // Bloquear antes de leer el tamaño
    int size = list->size;
    pthread_mutex_unlock(&list->lock);  // Desbloquear después de leer el tamaño
    
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);  // Bloquear antes de manipular la lista
    if (list->head == NULL) {
        // Lista vacía
        pthread_mutex_unlock(&list->lock);  // Desbloquear antes de retornar
        return 0;  // Indicar error
    }

    if (index > list->size)
        index = list->size - 1;

    if (index < 0)
        index = 0;


    Node* current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    *out_value = current->data;

    pthread_mutex_unlock(&list->lock);  // Desbloquear después de manipular la lista
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);  // Bloquear antes de manipular la lista
    if (index > list->size)
        index = list->size - 1 ;

    if (index < 0)
        index = 0;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;

    if (index == 0) {
        // Insertar al principio
        new_node->next = list->head;
        list->head = new_node;
    } else {
        // Insertar en algún lugar diferente al principio
        Node* current = list->head;
        for (size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    list->size++;

    pthread_mutex_unlock(&list->lock);  // Desbloquear después de manipular la lista
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);  // Bloquear antes de manipular la lista
    if (list->head == NULL) {
        // Lista vacía
        pthread_mutex_unlock(&list->lock);  // Desbloquear antes de retornar
        return 0;  // Indicar error
    }

    if (index > list->size)
        index = list->size - 1;

    if (index < 0)
        index = 0;

    Node* temp;

    if (index == 0) {
        // Eliminar el primer elemento
        temp = list->head;
        list->head = list->head->next;
    } else {
        // Eliminar algún elemento diferente al principio
        Node* current = list->head;
        for (size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        temp = current->next;
        current->next = temp->next;
    }

    *out_value = temp->data;
    free(temp);
    list->size--;

    pthread_mutex_unlock(&list->lock);  // Desbloquear después de manipular la lista

    return 0;
}
