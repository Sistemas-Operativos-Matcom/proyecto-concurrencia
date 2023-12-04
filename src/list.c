#include "list.h"
#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"

int size = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int_ll_t *add_element(int value, int_ll_t *next)
{
    int_ll_t *new = (int_ll_t *)malloc(sizeof(int_ll_t));
    new->value = value;
    new->next = next;
    return new;
}

void destroy(int_ll_t *current)
{
    if(current->next)
        destroy(current->next);
    
    free(current);
    return;
}

// Init list structure
int init_list(int_ll_t *list)
{
    list = add_element(0, NULL);

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&mutex);

    if(list->next)
        destroy(list->next);

    size = 0;
    
    free(list);

    pthread_mutex_unlock(&mutex);
    
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&mutex);

    if(size == 0)
    {
        *out_value = 1;
        pthread_mutex_unlock(&mutex);
        return 1;
    }


    index = (index < 0) ? 0 : (index >= size) ? size - 1 : index;

    int_ll_t *current = list;

    while(index > 0)
    {
        current = current->next;
        index--;
    }

    *out_value = current->next->value;

    pthread_mutex_unlock(&mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&mutex);
    
    index = (index < 0) ? 0 : (index >= size) ? size - 1 : index;

    int_ll_t *current = list;

    while(index > 0)
    {
        current = current->next;
        index--;
    }

    current->next = add_element(value, current->next);
    size++;

    pthread_mutex_unlock(&mutex);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&mutex);

    if(size == 0)
    {
        *out_value = 1;
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    
    
    index = (index < 0) ? 0 : (index >= size) ? size - 1 : index;

    int_ll_t *current = list;

    while(index > 0)
    {
        current = current->next;
        index--;
    }

    int_ll_t *old = current->next;
    current->next = current->next->next;
    
    *out_value = old->value;

    free(old);

    size--;

    pthread_mutex_unlock(&mutex);

    return 0;
}
