#include "list.h"
#include <stdlib.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    node_t *current = list->head;
    node_t *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->size = 0;
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    int size = list->size;
    pthread_mutex_unlock(&list->lock);
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    if(list->size==0)
        return 1;
    
    pthread_mutex_lock(&list->lock);
    if (index < 0)
        index = 0;
    if (index >= list->size)
        index = list->size - 1;

    node_t *current = list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    *out_value = current->value;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    new_node->value = value;

    if (index == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        node_t *current = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }

        new_node->next = current->next;
        current->next = new_node;
    }

    list->size++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    if(list->size==0)
        return 1;

    pthread_mutex_lock(&list->lock);
    if (index < 0)
        index = 0;
    if (index >= list->size)
        index = list->size - 1;

    node_t *to_remove;
    if (index == 0)
    {
        to_remove = list->head;
        list->head = to_remove->next;
    }
    else
    {
        node_t *current = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }

        to_remove = current->next;
        current->next = to_remove->next;
    }

    *out_value = to_remove->value;
    free(to_remove);
    list->size--;
    pthread_mutex_unlock(&list->lock);
    return 0;
}