#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int_ll_t *new_list()
{
    int_ll_t *new_list = (int_ll_t *)malloc(sizeof(int_ll_t));
    new_list->size = 0;
    new_list->root = NULL;
    return new_list;
}

node_t *new_node(int val)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->val = val;
    return node;
}

// Init list structure
int init_list(int_ll_t *list)
{
    list = new_list();
    int x = pthread_mutex_init(&(list->lock), NULL);
    return x;
}

// Free list structure
int free_list(int_ll_t *list)
{
    node_t *current = list->root;
    while (current != NULL)
    {
        node_t *next = current->next;
        free(current);
        current = next;
    }

    int x = pthread_mutex_destroy(&(list->lock));
     if (!x)
        free(list);
    return x;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->lock));
    int size = list->size;
    pthread_mutex_unlock(&(list->lock));
    return size;
}
node_t *get_item(int_ll_t *list, int index)
{
    if (list->root == NULL || index <= 0)
        return list->root;
    else
    {
        int c = 0;
        node_t *current = list->root;
        while (current->next != NULL && c < index)
        {
            c += 1;
            current = current->next;
        }
        return current;
    }
}
// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    node_t *current = get_item_at(list, index);
    if (current == NULL)
    {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    else
        *out_value = current->val;
    pthread_mutex_unlock(&(list->lock));

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
   pthread_mutex_lock(&(list->lock));

    if (list->root == NULL || index <= 0)
    {

        node_t *next_to_root = list->root;
        list->root = create_node(value);
        list->root->next = next_to_root;
    }
    else
    {
        node_t *prev = get_item_at(list, index - 1);
        node_t *x = prev->next;
        prev->next = create_node(value);
        prev->next->next = x;
    }
    list->size++;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));

    if (list->root != NULL)
    {
        list->size--;
        if (index <= 0)
        {

            node_t *deleted = list->root;
            *out_value = deleted->val;
            list->root = list->root->next;
            free(deleted);
        }
        else
        {
            node_t *prev = get_item_at(list, index - 1);
            while (prev->next == NULL && index > 0)
            {
                index -= 1;
                prev = get_item_at(list, index - 1);
            }
            if (index <= 0)
            {
                node_t *deleted = list->root;
                *out_value = deleted->val;
                list->root = list->root->next;
                free(deleted);
            }
            else
            {
                node_t *deleted = prev->next;
                *out_value = deleted->val;
                prev->next = prev->next->next;
                free(deleted);
            }
        }
    }
    else
    {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    pthread_mutex_unlock(&(list->lock));

    return 0;
}
