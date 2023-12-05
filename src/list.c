#include "list.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

Node *get_node(int index, int_ll_t *list)
{
    if (index < 0 || index > list->size - 1)
        return NULL;

    Node *node = list->head;
    int i = 0;
    while (i < index)
    {
        node = node->next;
        i++;
    }
    return node;
}
// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);

    Node *current = list->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    pthread_mutex_unlock(&list->lock);
    return list->size;
    ;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    index = index_fixer(index, list->size);
    Node *node = get_node(index, list);
    *out_value = node->data;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);

    index = index_fixer(index, list->size);

    Node *new_node = malloc(sizeof(Node));
    new_node->data = value;

    if (index <= 0)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        Node *newPrev = get_node(index - 1, list);
        Node *newNext = newPrev->next;

        new_node->next = newNext;
        new_node->prev = newPrev;

        newNext->prev = new_node;
        newPrev->next = new_node;
    }

    list->size++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    index = index_fixer(index, list->size);
    Node *current_node = list->head;

  
    if (current_node == NULL)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if (index == 0)
    {
        list->head = list->head->next;
    }

    else
    {
        Node *previous = get_node(index - 1, list);
        current_node = get_node(index, list);
        previous->next = current_node->next;
    }

    *out_value = current_node->data;
    free(current_node);

    list->size--;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

int index_fixer(int index, int size)
{
    if (index < 0)
        index = 0;
    else if (index >= size)
        index = size - 1;
    return index;
}


