#include "list.h"
#include <stdlib.h>
#include <stdio.h>
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
    node *cur = list->head;
    while (cur != NULL)
    {
        node *next = cur->next;
        free(cur);
        cur = next;
    }
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    int siz = list->size;
    pthread_mutex_unlock(&list->lock);
    return siz;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    if (index < 0)
    {
        index = 0;
    }
    if (index > list->size - 1)
    {
        index = list->size - 1;
    }
    node *cur = list->head;

    for (int j = 0; j < index; j++)
    {
        cur = cur->next;
    }

    *out_value = cur->key;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    if (index < 0)
    {
        index = 0;
    }
    if (index >= list->size)
    {
        index = list->size;
    }
    if (index == 0)
    {
        node *temp = list->head;
        list->head = (node *)malloc(sizeof(node));
        list->head->key = value;
        list->head->next = temp;
    }
    else
    {
        // insert at index > 0, implies find before node to update references.
        node *before = list->head;
        int actual_index = 0;
        while (actual_index++ < index - 1)
        {
            before = before->next;
        }
        node *temp = before->next;
        before->next = (node *)malloc(sizeof(node));
        before->next->key = value;
        before->next->next = temp;
    }
    list->size++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    if (index < 0)
    {
        index = 0;
    }
    if (index > list->size - 1)
    {
        index = list->size - 1;
    }
    if (index == 0)
    {
        node *to_delete = list->head;
        *out_value = to_delete->key;
        list->head = to_delete->next;
        free(to_delete);
    }
    else
    {
        // delete at index > 0, implies find before node to update references.
        node *before = list->head;
        int actual_index = 0;
        while (actual_index++ < index - 1)
        {
            before = before->next;
        }
        node *temp = before->next;
        *out_value = temp->key;
        before->next = temp->next;
        free(temp);
    }
    list->size--;
    pthread_mutex_unlock(&list->lock);
    return 0;
}