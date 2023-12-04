#include "list.h"
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list = (int_ll_t *)malloc(sizeof(int_ll_t));
    list->head =  NULL;
    list->size = 0;
    return pthread_mutex_init(&(list->mutex), NULL);
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    free(list);
    pthread_mutex_unlock(&(list->mutex));
    return pthread_mutex_destroy(&(list->mutex));
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    int size = list->size;
    pthread_mutex_unlock(&(list->mutex));

    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));

    if(list->size == 0 || list == NULL)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    ll_node_t *current_node = list->head;
    int current_index = 0;

    while( current_node->next_node != NULL && current_index < index )
    {
        current_node = current_node->next_node;
        current_index++;
    }

    *out_value = current_node->value;
    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));
    ll_node_t *current_node = list->head;
    ll_node_t *new_node = (ll_node_t *)malloc(sizeof(ll_node_t));

    if(index <= 0 || list->size == 0)
    {
       new_node->value = value;
       new_node->next_node = current_node;
       list->head = new_node;
    }
    else if (index < list->size)
    {
        // tomemos current_node como previous_node
        ll_node_t *next_current_node = current_node->next_node;

        for(int i = 1;i < index;i++)
        {
            current_node = next_current_node;
            next_current_node = next_current_node->next_node;
        }

        new_node->value = value;
        new_node->next_node = next_current_node;
        current_node->next_node = new_node;
    }
    else 
    {
        while(current_node->next_node != NULL)
        {
            current_node = current_node->next_node;
        }

        new_node->value = value;
        new_node->next_node = NULL;
        current_node->next_node = new_node;
    }

    list->size++;
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));

    if(list->size == 0 || list == NULL)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }

    ll_node_t *current_node = list->head;

    if(index <= 0 || list->size == 1)
    {
        *out_value = list->head->value;
        list->head=current_node->next_node;
        free(current_node);
    }
    else 
    {
        // tomemos current_node como previous_node
        ll_node_t *next_current_node = current_node->next_node;

        int i = 1;
        while ( i < index & next_current_node->next_node != NULL )
        {
            current_node = next_current_node;
            next_current_node = next_current_node->next_node;
            i++;
        }

        *out_value = next_current_node->value;
        current_node->next_node = next_current_node->next_node;
        free(next_current_node);
    }

    list->size--;
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}