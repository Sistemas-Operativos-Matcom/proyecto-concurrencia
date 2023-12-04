#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int_ll_t *new_list()
{
    int_ll_t *new_list = (int_ll_t *)malloc(sizeof(int_ll_t));
    new_list->size = 0;
    new_list->first = NULL;

    return new_list;
}

int_ll_node_t *new_node(int value)
{
    int_ll_node_t *node = (int_ll_node_t *)malloc(sizeof(int_ll_node_t));
    node->val = value;

    return node;
}

// Init list structure
int init_list(int_ll_t *list)
{
    list = new_list();
    pthread_mutex_init(&(list->mutex), NULL);
    
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    int_ll_node_t *node = list->first;

    while (node != NULL)
    {
        int_ll_node_t *_aux_node = node;
        node = node->next;
        free(_aux_node);
    }

    pthread_mutex_destroy(&(list->mutex));

    free(list);

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->size;
}

int_ll_node_t *get_node(int_ll_t *list, int index, int is_delete)
{
    int i = 0;
    int_ll_node_t *node = list->first;

    if (is_delete)
    {
        while (node->next->next && i < index)
        {
            node = node->next;
            i++;
        }   
    }
    else
    {
        while (node->next && i < index)
        {
            node = node->next;
            i++;
        }
    }

    return node;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));

    int_ll_node_t *node = get_node(list, index, 0);

    if (!node)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    *out_value = node->val;

    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));

    if (!list->first)
    {
        list->first = new_node(value);
        list->first->next = NULL;
    }
    else
    {
        int_ll_node_t *node = get_node(list, index - 1, 0);
        int_ll_node_t *next_node = node->next;
        node->next = new_node(value);
        node->next->next = next_node;
    }

    list->size++;

    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));

    if (!list->first)
    {

        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    list->size--;

    int_ll_node_t *node = get_node(list, index - 1, 1);

    int_ll_node_t *_aux_node = node->next;
    
    *out_value = _aux_node->val;
    node->next = node->next->next;

    free(_aux_node);

    pthread_mutex_unlock(&(list->mutex));

    return 0;
}