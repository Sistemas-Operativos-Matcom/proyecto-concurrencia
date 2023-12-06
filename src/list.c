#include <stdlib.h>
#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    

    return pthread_mutex_init(&list->lock, NULL);;
}

// Free list structure
int free_list(int_ll_t *list)
{
    _int_node_t *current = list->head;
    while (current != NULL)
    {
        _int_node_t *next = current->next;
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
    
}

// Get list size
int size_list(int_ll_t *list)
{
    int lockResult = pthread_mutex_lock(&(list->lock));
    int size = list->size;
    pthread_mutex_unlock(&(list->lock));
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    int lockResult = pthread_mutex_lock(&(list->lock));
    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    if (index >= list->size)
        index = list->size - 1;
    if (index<0)
        index = 0;
    
    _int_node_t *result = list->head;
    for (size_t i = 0; i < index; i++)
    {
        result = result->next;
    }
    *out_value = result->data;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    int lockResult = pthread_mutex_lock(&(list->lock));
    if (index > list->size)
        index = list->size;
    if (index<0)
        index = 0;
    
    _int_node_t *new_node = malloc(sizeof(_int_node_t));
    new_node->data = value;
    if (index == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        _int_node_t *result = list->head;
        for (size_t i = 0; i < index -1; i++)
        {
            result = result->next;
        }
        new_node->next = result->next;
        result->next = new_node;
    }
    list->size++;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    int lockResult = pthread_mutex_lock(&(list->lock));
    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    if (index >= list->size)
        index = list->size - 1;
    if (index<0)
        index = 0;
    
    if (index==0)
    {
        _int_node_t *result = list->head;
        list->head = result->next;
        *out_value = result->data;
        free(result);
    }
    else
    {
        _int_node_t *result = list->head;
        for (size_t i = 0; i < index -1; i++)
        {
            result = result->next;
        }
        _int_node_t *to_remove = result->next;
        if (to_remove->next == NULL)
        {
            result->next = NULL;
        }else
        {
            result->next = to_remove->next;

        }
        *out_value = to_remove->data;
        free(to_remove);
    }
    list->size--;
    pthread_mutex_unlock(&(list->lock));

    
    return 0;
}
