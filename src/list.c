#include "list.h"
#include <stddef.h>

// Init list structure
int init_list(int_ll_t *list)
{
    // TODO: Your code here!
    list->first = NULL;
    list->length = 0;
    int init = pthread_mutex_init(&list->lock, NULL);
    return init;
}

// Free list structure
int free_list(int_ll_t *list)
{
     node *actual = list->first;
    while (actual != NULL) {
        node *tmp = actual->next;
        free(actual);
        actual = tmp;
    }
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->lock));
    int length = list->length;
    pthread_mutex_unlock(&(list->lock));
    return length;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    if (list->length == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    if (index >= list->length) index = list->length-1;
    if (index < 0) index = 0;
    node *result = list->first;
    for (size_t i = 0; i < index; i++) result = result->next;
    *out_value = result->value;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->lock));
    node *tmp = malloc(sizeof(node));
    tmp->value = value; 
    if (index > list->length) index = list->length;
    if (index < 0) {
        tmp->next = list->first;
        list->first = tmp;
        list->length++;
        pthread_mutex_unlock(&(list->lock));
        return 0;
    }
    node *result = list->first; 
    for (size_t i = 0; i < index-1; i++) result = result->next;
    tmp->next = result->next;
    result->next = tmp;
    list->length++;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    if (list->length == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    if (index >= list->length) index = list->length - 1;
    if (index < 0) {
        node *result = list->first;
        list->first = result->next;
        *out_value = result->value;
        free(result);
        list->length--;
        pthread_mutex_unlock(&(list->lock));
        return 0;
    }
    node *result = list->first;
    for (size_t i = 0; i < index-1; i++) result=result->next;
    node *remove = result->next;
    if (remove->next == NULL) result->next = NULL;
    else result->next = remove->next;
    *out_value = remove->value;
    free(remove);
    list->length--;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}
