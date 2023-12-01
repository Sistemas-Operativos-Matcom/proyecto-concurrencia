#include "list.h"

#include <pthread.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAIL 1

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

pthread_mutex_t lock;

// Init list structure
int init_list(int_ll_t *list)
{
    list->size = 0;
    list->head = NULL;

    return pthread_mutex_init(&(list->lock), NULL);
}

// Free list structure
int free_list(int_ll_t *list)
{
    int_node_t* iter = list->head;

    while (iter != NULL) {
        int_node_t* tmp = iter->next;
        free(iter);
        iter = tmp;
    }

    int status = pthread_mutex_destroy(&(list->lock));
    free(list);

    return status;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->lock));
    
    int size = list->size;
    
    pthread_mutex_unlock(&(list->lock));
    
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));

    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));
        return FAIL;
    }

    index = max(index, 0);
    index = min(index, list->size-1);

    int_node_t* node = list->head;

    while (index --) {
        node = node->next;
    }

    *out_value = node->value;

    pthread_mutex_unlock(&(list->lock));

    return SUCCESS;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->lock));
    
    index = max(index, 0);
    index = min(index, list->size);

    int_node_t* node = (int_node_t*) malloc(sizeof(int_node_t));
    node->value = value;
    node->next = node->prev = NULL;

    if (index == 0) {
        node->next = list->head;

        if (list->size != 0) {
            list->head->prev = node;
        }

        list->head = node;
        list->size ++;
    }

    else {
        int_node_t* iter = list->head;

        while (--index > 0) {
            iter = iter->next;
        }
        
        int_node_t* tmp = iter->next;
        
        iter->next = node;
        node->prev = iter;
        node->next = tmp;
        if (tmp != NULL)tmp->prev = node;

        list->size ++;
    }
    
    pthread_mutex_unlock(&(list->lock));

    return SUCCESS;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    
    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));
        return FAIL;
    }

    index = max(index, 0);
    index = min(index, list->size-1);

    int_node_t* iter = list->head;

    while (index-- > 0) {
        iter = iter->next;
    }

    *out_value = iter->value;

    int_node_t* prev = iter->prev;
    int_node_t* next = iter->next;

    if (prev != NULL && next != NULL) {
        prev->next = next;
        next->prev = prev;
    }
    else if (prev != NULL && next == NULL) {
        prev->next = NULL;
    }
    else if (prev == NULL && next != NULL) {
        next->prev = NULL;
        list->head = next;
    }

    free(iter);
    list->size--;

    if (list->size == 0)list->head = NULL;
    
    pthread_mutex_unlock(&(list->lock));

    return SUCCESS;
}
