#include "list.h"
#include <stddef.h>
#include <stdlib.h>

// Init list structure
int init_list(int_ll_t *list)
{
    if (!list) return 1;
    pthread_mutex_init(&list->mutex, NULL);
    list->head = NULL;
    list->size = 0;
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    if (!list) return 1;
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;
    node_t *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
    pthread_mutex_unlock(&list->mutex);
    pthread_mutex_destroy(&list->mutex);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    if (!list) return 1;
    pthread_mutex_lock(&list->mutex);
    int size = list -> size;
    pthread_mutex_unlock(&list->mutex);
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    if (!list || index < 0 || index >= list->size) return 1;
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    *out_value = current->data;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    if (!list || index < 0 || index > list->size) return 1;
    pthread_mutex_lock(&list->mutex);
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node) {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }
    new_node->data = value;
    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        node_t *current = list->head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    list->size++;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    if (!list || index < 0 || index >= list->size) return 1;
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;
    node_t *temp_node = NULL;

    if (index == 0) {
        temp_node = list->head;
        *out_value = temp_node->data;
        list->head = list->head->next;
    } else {
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        temp_node = current->next;
        *out_value = temp_node->data;
        current->next = temp_node->next;
    }
    free(temp_node);
    list->size--;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}
