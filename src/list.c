#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// metodos chungos mios
node_t *create_node(int val)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->val = val;
    return new_node;
}

int_ll_t *create_list()
{
    int_ll_t *new_list = (int_ll_t *)malloc(sizeof(int_ll_t));
    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}

node_t *get_item_at(int_ll_t *list, int index)
{
    if (list->head == NULL || index <= 0)
        return list->head;
    else
    {
        int c = 0;
        node_t *curr = list->head;
        while (curr->next != NULL && c < index)
        {
            c += 1;
            curr = curr->next;
        }
        return curr;
    }
}

// Init list structure
int init_list(int_ll_t *list)
{
    list = create_list();
    int x = pthread_mutex_init(&(list->lock), NULL);
    return x;
}

// Free list structure
int free_list(int_ll_t *list)
{
    node_t *curr = list->head;
    while (curr != NULL)
    {
        node_t *next = curr->next;
        free(curr);
        curr = next;
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
    return size > 0 ? size : 1;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    node_t *curr = get_item_at(list, index);
    if (curr == NULL)
    {
        //*out_value = 1;
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    else
        *out_value = curr->val;
    pthread_mutex_unlock(&(list->lock));

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->lock));

    if (list->head == NULL || index <= 0)
    {

        node_t *next_to_head = list->head;
        list->head = create_node(value);
        list->head->next = next_to_head;
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

    if (list->head != NULL)
    {
        list->size--;
        if (index <= 0)
        {

            node_t *deleted = list->head;
            *out_value = deleted->val;
            list->head = list->head->next;
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
                node_t *deleted = list->head;
                *out_value = deleted->val;
                list->head = list->head->next;
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
