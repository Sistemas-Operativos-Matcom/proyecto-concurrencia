#include "list.h"
#include <stdlib.h>
#include <pthread.h>
// Init list structure
int init_list(int_ll_t *list)
{
    list = (int_ll_t*) malloc(sizeof(int_ll_t));
    list-> head = NULL;
    pthread_mutex_init(&(list->mutex), NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    nodeD* node =list->head;
        while (node != NULL) {
        nodeD* temp = node->next;
        free(node);
        node = temp;
    }
    
    pthread_mutex_destroy(&(list->mutex));
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));

    int count = 0;
    nodeD* node = list->head;

    
    while (node != NULL) {
        count++;

        node= node->next;
    
    }

    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));

    return count;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    //bloquea el mutex
    pthread_mutex_lock(&(list->mutex));
    
    nodeD* node = list->head;
    int count = 0;
    if(node != NULL){
        // Recorre la lista hasta el nodo del indice
        
        while (node->next != NULL && count < index) {
            node = node->next;
            count++;
        }
    }

    *out_value = (node != NULL) ? node->value : 0;  

    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));


    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));
    // Caso especial: Insertar al principio de la lista en caso de que
    //el indice sea 0 o negativo
    if (index <= 0 || list->head == NULL) {
        nodeD* temp = list->head;
        list->head = (nodeD*)malloc(sizeof(nodeD));
        list->head->next = temp;
        list->head->value = value;
    } 
    else {
        nodeD* node = list->head;
        int count = 0;
        // Busca el nodo anterior al lugar donde se insertará el nuevo nodo
        
        
        while (node->next != NULL && count < index-1) {
            node = node->next;
            
            count++;
        }
        // Inserta el nuevo nodo después del nodo actual
        nodeD* temp = node->next;
        node->next = (nodeD*)malloc(sizeof(nodeD));
        node->next->value = value;
        node->next->next = temp;     
    }
    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));

    int ret = list->head == NULL; // Lista vacía
    if(list->head != NULL){
        // Caso especial: Eliminar el primer elemento de la lista
        
        if(index <= 0){
            nodeD* tmp = list->head;
            *out_value = tmp->value;
            list->head = list->head->next;

            free(tmp);
        }
        else{
            nodeD* node = list->head;
            int count = 0;
            // Busca el nodo en el índice anterior al que se eliminará
            
            while (node->next != NULL && count < index-1) {
                if (node->next->next == NULL){      // Si llega al final de la lista
                    break;
                }
                node = node->next;
                count++;     
            }
            // Elimina el nuevo nodo después del nodo actual
           
            nodeD* tmp = node->next;

            *out_value = tmp->value;

            node->next = node->next->next;

            free(tmp);
        }
    }
    // Desbloquea el mutex después de acceder a la lista
    pthread_mutex_unlock(&(list->mutex));


    return ret;
}
