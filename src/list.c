#include "list.h"
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
int init_list(int_ll_t *list)
{
    list->size = 0;
    list->NextNode = NULL;
    list->value = -1;
    pthread_mutex_init(&lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&lock);
    int count = 0;
    int_ll_t *n = list;
    int_ll_t *n2 = list->NextNode;
    while (count < list->size)
    {
        free(n);
        n = n2;
        n2 = n2->NextNode;
    }
    pthread_mutex_unlock(&lock);  
    pthread_mutex_destroy(&lock);
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    if (list->size == 0)
    {
        return 1;
    }
    if (index >= list->size)
    {
        index = list->size - 1;
    }
    int count = 0;
    int_ll_t *curr = list;
    while (count < index)
    {
        curr = curr->NextNode;
        count ++;
    }
    *out_value = curr->value;
    pthread_mutex_unlock(&lock); 
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    if (index == 0)
    {
        list->value = value;
        return 0;
    }
    if (index > list->size)
    {
        index = list->size;
    }
    int count = 0;
    int_ll_t *curr = list;
    while (count < index - 1)
    {
        curr = curr->NextNode;
        count ++;
    }
    int_ll_t *temp = curr->NextNode;
    int_ll_t *new = malloc(sizeof(int_ll_t));
    curr->NextNode = new;
    new->value = value;
    new->NextNode = temp;
    list->size ++;
    pthread_mutex_unlock(&lock); 
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&lock);
    if (index == 0)
    {
        *out_value = list->value;
        int size = list->size -1;
        list = list->NextNode;
        list->size = size;
        return 0;
    }
    if (index > list->size)
    {
        index = list->size;
    }
    int count = 0;
    int_ll_t *curr = list;
    while (count < index - 1)
    {
        curr = curr->NextNode;
        count ++;
    }
    int_ll_t *temp = curr->NextNode;
    curr->NextNode = temp->NextNode;
    list->size --;
    pthread_mutex_unlock(&lock); 
    return 0;
}
