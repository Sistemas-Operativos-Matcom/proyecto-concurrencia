#include <stdio.h>
#include <stdlib.h>
// #include <semaphore.h>
#include <pthread.h>

#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list = malloc(sizeof(int_ll_t));
    list->size = 0;
    list->nodes = malloc(sizeof(ll_node_t));

    pthread_mutex_init(&list->mutex, NULL);

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->mutex);
    list->size = 0;
    list->nodes = NULL;
    pthread_mutex_unlock(&list->mutex);

    pthread_mutex_destroy(&list->mutex);

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->mutex);
    int size = list->size;
    pthread_mutex_unlock(&list->mutex);

    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    ll_node_t *current = list->nodes;

    if(current == NULL) {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    pthread_mutex_unlock(&list->mutex);

    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        current = (ll_node_t *) current->next;
    }

    pthread_mutex_lock(&list->mutex);
    *out_value = current->value;
    pthread_mutex_unlock(&list->mutex);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    ll_node_t *new = malloc(sizeof(ll_node_t));
    new->value = value;
    
    pthread_mutex_lock(&list->mutex);
    ll_node_t *current = list->nodes;

    if(current == NULL) {
        list->nodes = new;
        list->size++;
        pthread_mutex_unlock(&list->mutex);
        return 0;
    }

    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        current = (ll_node_t *) current->next;
    }

    if(current->next == NULL) {
        current->next = (struct ll_node_t *) new;
    } else {
        ll_node_t *next = (ll_node_t *) current->next;
        new->next = (struct ll_node_t *) next;
        current->next = (struct ll_node_t *) new;
    }

    list->size++;
    pthread_mutex_unlock(&list->mutex);

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);
    ll_node_t *current = list->nodes;
    ll_node_t *prev = NULL;

    if(current == NULL) {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }
    
    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        prev = current;
        current = (ll_node_t *) current->next;
    }

    if(current == NULL) {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    if(prev == NULL) {
        list->nodes = (ll_node_t *) current->next;
    } else {
        prev->next = (struct ll_node_t *) current->next;
    }

    *out_value = current->value;
    list->size--;
    pthread_mutex_unlock(&list->mutex);

    return 0;
}
