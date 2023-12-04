#include "list.h"

#include<stdlib.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list = (int_ll_t*) malloc(sizeof(int_ll_t));

    list->begin = NULL;
    list->end = NULL;
    list->size = 0;
    int success = pthread_mutex_init(&(list->lock), NULL);

    return success;
}

// Free list structure
int free_list(int_ll_t *list)
{
    node_t* current = list->begin;
    node_t* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    int success = pthread_mutex_destroy(&(list->lock));
    free(list);
    
    return success;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->lock));
    
    int list_size = list-> size;

    pthread_mutex_unlock(&(list->lock));

    return list_size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    
    if (index < 0) {
        index = 0;
    }
    else if (index >= list->size) {
        index = list->size - 1;
    }

    node_t* current = list->begin;
    
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    *out_value = current->value;

    int success = pthread_mutex_unlock(&(list->lock));

    return success;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->lock));

    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;
    
    if (index <= 0) {
        new_node->next = list->begin;
        list->begin = new_node;

        if (list->size == 0) {
            list->end = new_node;
        }
    } 
    else if (index >= list->size) {
        list->end->next = new_node;
        list->end = new_node;
    } 
    else {
        node_t* current = list->begin;

        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        
        new_node->next = current->next;
        current->next = new_node;
    }

    list->size++;

    pthread_mutex_unlock(&(list->lock));
        
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value) {
    pthread_mutex_lock(&(list->lock));

    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));

        return -1;
    }

    node_t* current = list->begin;
    if (index <= 0) {
        list->begin = current->next;
        if (list->size == 1) {
            list->end = NULL;
        }
    } 
    else if (index >= list->size - 1) {
        while (current->next != list->end) {
            current = current->next;
        }
        list->end = current;
        current = current->next;
    } 
    else {
        node_t* temp = list->begin;

        for (int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        node_t* next = temp->next->next;
        current = temp->next;
        temp->next = next;
    }

    *out_value = current->value;
    free(current);
    list->size--;

    pthread_mutex_unlock(&(list->lock));

    return 0;
}
