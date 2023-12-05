#include "list.h"
#include <stdio.h>
#include <stdlib.h>
node_t *new_node(int value, node_t *link)
{
    node_t *new = (node_t *)malloc(sizeof(node_t));
    new->link = link;
    new->value = value;
    return new;
}

// Init list structure
int init_list(int_ll_t *list)
{
    // list = (int_ll_t *)malloc(sizeof(int_ll_t));
    pthread_mutex_init(&list->exmut, NULL);
    list->head = NULL;
    list->size = 0;
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    // pthread_mutex_lock(&list->exmut);
    node_t *node = list->head;

    while (node != NULL)
    {
        node_t *link = node->link;
        free(node);
        node = link;
    }
    free(list);
    // pthread_mutex_unlock(&list->exmut);
    pthread_mutex_destroy(&list->exmut);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->exmut);
    int size = list->size;
    pthread_mutex_unlock(&list->exmut);
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->exmut);
    if (list == NULL || list->size == 0)
    {
        pthread_mutex_unlock(&list->exmut);
        return 1;
    }

    node_t *node = list->head;
    if (index >= list->size)
        index = list->size - 1;
    else if (index < 0)
    {
        index = 0;
    }
    for (int i = 0; i < index; i++)
    {
        if (node != NULL)
        {
            node = node->link;
        }
    }
    *out_value = node->value;
    pthread_mutex_unlock(&list->exmut);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->exmut);
    if (list == NULL)
    {
        pthread_mutex_unlock(&list->exmut);
        return 1;
    }
    node_t *node = list->head;
    if (index > list->size)
    {
        index = list->size;
    }
    if (list->head == NULL || index <= 0)
    {
        node_t *new = new_node(value, list->head);
        list->head = new;
        list->size++;
        pthread_mutex_unlock(&list->exmut);
        return 0;
    }
    for (int i = 0; i < index - 1; i++)
    {
        if (node != NULL)
        {
            node = node->link;
        }
    }
    node_t *new = new_node(value, node->link);
    node->link = new;
    list->size++;
    pthread_mutex_unlock(&list->exmut);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->exmut);
    if (list == NULL || list->size == 0)
    {
        pthread_mutex_unlock(&list->exmut);
        return 1;
    }
    if (index >= list->size)
    {
        index = list->size - 1;
    }

    node_t *node = list->head;
    if (index <= 0)
    {
        *out_value = node->value;
        list->head = list->head->link;
        free(node);
    }
    else
    {
        for (int i = 0; i < index - 1; i++)
        {
            if (node != NULL)
            {
                node = node->link;
            }
        }
        node_t *link = node->link;
        node->link = link->link;
        *out_value = link->value;
        free(link);
    }
    list->size--;
    pthread_mutex_unlock(&list->exmut);
    return 0;
}
