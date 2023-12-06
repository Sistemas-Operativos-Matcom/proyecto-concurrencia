#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include "list.h"

int normalize_index(int index, int size)
{
    return index <= 0 ? 0 : (index >= size ? size - 1 : index);
}

// Init list structure
int init_list(int_ll_t *list)
{
    link_node *new_ptr = malloc(sizeof(link_node));
    *new_ptr = (link_node){0, NULL};
    list->root = new_ptr;
    list->length = 0;
    pthread_mutex_init(&list->lock, NULL);

    return 0;
}

void free_list_r(link_node *node)
{
    if (node->next_node != NULL)
        free_list_r(node->next_node);
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

    index = normalize_index(index, list->length);

    if (list->length == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    link_node *actual_node = list->root;
    for (size_t i = 0; i < index; i++)
    {
        actual_node = actual_node->next_node;
    }
    *out_value = actual_node->value;

    pthread_mutex_unlock(&list->lock);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{

    pthread_mutex_lock(&list->lock);

    index = normalize_index(index, list->length);

    if (index == 0)
    {
        link_node *new_ptr = malloc(sizeof(link_node));
        *new_ptr = (link_node){value, list->root};
        list->root = new_ptr;
    }

    else
    {
        link_node *actual_node = list->root;
        for (size_t i = 0; i < index - 1; i++)
        {
            actual_node = actual_node->next_node;
        }
        link_node *new_ptr = malloc(sizeof(link_node));
        *new_ptr = (link_node){value, actual_node->next_node};
        actual_node->next_node = new_ptr;
    }

    list->length++;

    pthread_mutex_unlock(&list->lock);

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{

    pthread_mutex_lock(&list->lock);
    index = normalize_index(index, list->length);

    if (list->length <= 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    if (index == 0)
    {
        link_node *temp = list->root;
        list->root = list->root->next_node;
        *out_value = temp->value;
        free(temp);
    }
    else
    {

        link_node *actual_node = list->root;

        for (size_t i = 0; i < index - 1; i++)
        {
            actual_node = actual_node->next_node;
        }
        *out_value = actual_node->next_node->value;
        if (actual_node->next_node->next_node == NULL)
        {
            free(actual_node->next_node);
            actual_node->next_node = NULL;
        }
        else
        {
            link_node *new_next_node = actual_node->next_node->next_node;
            free(actual_node->next_node);
            actual_node->next_node = new_next_node;
        }
    }
    list->length--;

    pthread_mutex_unlock(&list->lock);

    return 0;
}
