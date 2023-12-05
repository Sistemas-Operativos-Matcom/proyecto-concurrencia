#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int check_list_index(int index, int size)
{
    if(index => size) index = size-1;
    else if(index < 0) index = 0;

    return index;
}

// Init list structure
int init_list(int_ll_t *list)
{
    int_ll_t *list = (int_ll_t*)malloc(sizeof(int_ll_t));
    list->size = 0;
    list->head = NULL;
    return pthread_mutex_init(&(list->mutex), NULL);
}

// Free list structure
int free_list(int_ll_t *list)
{   
    pthread_mutex_lock(&list->mutex);
    list->size = 0; 

    while (list->head != NULL) {
        node_t* next = list->head;
        list->head = list->head->next;
        free(next);
    }

    pthread_mutex_unlock(&list->mutex);
    pthread_mutex_destroy(&list->mutex);

    free(list);

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->mutex);
    int size = list->size;
    pthread_mutex_unlock(&list->mutex);
    
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    
    if(list->head == NULL){
       pthread_mutex_unlock(&list->mutex);
       return 1; 
    }

    index = check_list_index(index, list->size);
    node_t *next = list->head;

    for(int i = 0; i < index; i++){
        next = next->next;
    }
    
    out_value = next->data;
    pthread_mutex_unlock(&list->mutex);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->mutex);
    index = check_list_index(index, list->size);

    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = value;

    list->size++;

    if(index == 0)
    {
        list->head = new_node;
    }else{
        node_t* next = list->head;
        for (int i = 0; i < index-1; i++) {
            next = next->next;
        }
        new_node->next = next->next;
        next->next = new_node;
    }
    pthread_mutex_unlock(&list->mutex);

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    if(list->head == NULL)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    index = check_list_index(index, list->size);
    list->size--;

    node_t *new_node = list->head;
    if (index == 0) list->head = list->head->next;
    else 
    {
        node_t *next = list->head;

        for (size_t i = 0; i < index - 1; ++i) {
            next = next->next;
        }

        new_node = next->next;
        next->next = new_node->next;
    }

    *out_value = new_node->data;

    pthread_mutex_unlock(&list->lock);
    free(new_node);

    return 0;
}
