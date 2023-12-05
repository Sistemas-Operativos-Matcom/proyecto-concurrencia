#include "list.h"
#include "semaphore.h"
#include <stdlib.h>
#include <stdio.h>
#include "pthread.h"
//PAULA
// Init list structure
int init_list(int_ll_t *list)
{
    list->size = 0;
    list->head = NULL;
    pthread_mutex_init(&list->mutex, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->mutex);
    node_t *node = list->head;

    // if (node == NULL)
    // {
    //     pthread_mutex_unlock(&list->mutex);
    //     return 1;
    // }

    while (node != NULL)
    {
        node_t *next = node->next;
        free(node);
        node = next;
    }

    list->head = NULL;
    list->size = 0;

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

node_t *get_node(int_ll_t *list, int index)
{
    if (index < 0 || (list->size) == 0)
        return NULL;

    if (index > (list->size) - 1)
        index = list->size - 1;

    node_t *node = list->head;
    int curr_index = 0;

    while (node != NULL)
    {
        if (curr_index == index)
            return node;
        node = node->next;
        curr_index++;
    }

    return node; // ojo
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    node_t *node = get_node(list, index);
    if (node == NULL)
        *out_value = (list->head)->value;
    else
        *out_value = node->value;

    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->mutex);
    node_t *prev_node = get_node(list, index - 1);
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    new_node->value = value;
    node_t *next_node;
    if (prev_node == NULL)
    {
        next_node = list->head;
        list->head = new_node;
    }
    else
    {
        next_node = prev_node->next;
        prev_node->next = new_node;
    }
    new_node->next = next_node;
    list->size++;

    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

   // pthread_mutex_lock(&list->mutex);
    node_t *prev_node = get_node(list, index = (index > list->size - 1) ? (list->size) - 2 : (index - 1));
    node_t *free_node;
    if (prev_node == NULL)
    {
        free_node = list->head;
        list->head = free_node->next;
        *out_value = free_node->value;
        free(free_node);
    }
    else
    {
        free_node = prev_node->next;
        prev_node->next = free_node->next;
        *out_value = free_node->value;
        free(free_node);
    }

    list->size--;

    pthread_mutex_unlock(&list->mutex);
    return 0;
}