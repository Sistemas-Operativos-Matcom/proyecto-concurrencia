#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include "list.h"

int fix_index(int index, int size)
{
    if (index < size)
        return 0;
    else if (index >= size)
        return size - 1;
    else
        return index;
}

// Init list structure
int init_list(int_ll_t *list)
{
    node *new = malloc(sizeof(node));
    new->value = 0;
    new->next = NULL;
    list->root = new;
    list->length = 0;
    pthread_mutex_init(&list->lock, NULL);

    return 0;
}

void free_list_r(node *node)
{
    if (node->next != NULL)
        free_list_r(node->next);
    free(node);
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    free_list_r(list->root);
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->length;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    index = fix_index(index, list->length);

    if (list->length == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    node *actual_node = list->root;
    for (size_t i = 0; i < index; i++)
    {
        actual_node = actual_node->next;
    }
    *out_value = actual_node->value;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    index = fix_index(index, list->length);

    node *new = malloc(sizeof(node));
    new->value = value;

    if (index == 0)
    {
        new->next = list->root;
        list->root = new;
    }
    else
    {
        node *actual_node = list->root;
        for (size_t i = 0; i < index - 1; i++)
        {
            actual_node = actual_node->next;
        }
        new->next = actual_node->next;
        actual_node->next = new;
    }
    list->length++;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    index = fix_index(index, list->length);

    if (list->length == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    if (index == 0)
    {
        node *temp = list->root;
        list->root = list->root->next;
        *out_value = temp->value;
        free(temp);
    }
    else
    {
        node *actual_node = list->root;

        for (size_t i = 0; i < index - 1; i++)
        {
            actual_node = actual_node->next;
        }
        *out_value = actual_node->next->value;
        if (actual_node->next->next == NULL)
        {
            free(actual_node->next);
            actual_node->next = NULL;
        }
        else
        {
            node *new_next_node = actual_node->next->next;
            free(actual_node->next);
            actual_node->next = new_next_node;
        }
    }
    list->length--;

    pthread_mutex_unlock(&list->lock);
    return 0;
}