#include "list.h"
#include <stdlib.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
     // Inicializa la lista como vacía
    list = (int_ll_t*) malloc(sizeof(int_ll_t));
    list->head = NULL;

    // Inicializa el mutex
    pthread_mutex_init(&(list->mutex), NULL);

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
     // Bloquea el mutex antes de acceder a la lista
    pthread_mutex_lock(&(list->mutex));

    node_t* curr = list->head;
    // Libera todos los nodos de la lista
    while (curr != NULL) {
        node_t* temp = curr->next;
        free(curr);
        curr = temp;
    }
   
    // Desbloquea el mutex antes de destruirlo
    pthread_mutex_unlock(&(list->mutex));

    // Destruye el mutex
    pthread_mutex_destroy(&(list->mutex));

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    // Bloquea el mutex antes de acceder a la lista
    pthread_mutex_lock(&(list->mutex));
    
    int count = 0;
    node_t* current = list->head;

    // Recorre la lista y cuenta los elementos
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));

    return count;  
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // Bloquea el mutex antes de acceder a la lista
    pthread_mutex_lock(&(list->mutex));

    node_t* current = list->head;
    int count = 0;
    if(current != NULL){
        // Recorre la lista hasta el índice deseado
        while (current->next != NULL && count < index) {
            current = current->next;
            count++;
        }
    }
   
    *out_value = (current != NULL) ? current->value : 0;  

    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // Bloquea el mutex antes de acceder a la lista
    pthread_mutex_lock(&(list->mutex));
    // Caso especial: Insertar al principio de la lista
    if (index <= 0 || list->head == NULL) {
        node_t* temp = list->head;
        list->head = (node_t*)malloc(sizeof(node_t));
        list->head->next = temp;
        list->head->value = value;
    } 
    else {
        node_t* current = list->head;
        int count = 0;
        // Busca el nodo en el índice anterior al lugar donde se insertará el nuevo nodo
        while (current->next != NULL && count < index-1) {
            current = current->next;
            count++;
        }
        // Inserta el nuevo nodo después del nodo actual
        node_t* temp = current->next;
        current->next = (node_t*)malloc(sizeof(node_t));
        current->next->value = value;
        current->next->next = temp;     
    }
    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    // Bloquea el mutex antes de acceder a la lista
    pthread_mutex_lock(&(list->mutex));

    int ret = list->head == NULL; // Lista vacía
    if(list->head != NULL){
        // Caso especial: Eliminar el primer elemento de la lista
        if(index <= 0){
            node_t* tmp = list->head;
            *out_value = tmp->value;
            list->head = list->head->next;
            free(tmp);
        }
        else{
            node_t* current = list->head;
            int count = 0;
            // Busca el nodo en el índice anterior al que se eliminará
            while (current->next != NULL && count < index-1) {
                if (current->next->next == NULL){      // Si llega al final de la lista
                    break;
                }
                current = current->next;
                count++;     
            }
            // Elimina el nuevo nodo después del nodo actual
            node_t* tmp = current->next;
            *out_value = tmp->value;
            current->next = current->next->next;
            free(tmp);
        }
    }
    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));

    return ret;  
}
