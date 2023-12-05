#include "list.h"
#include <stdio.h>
#include <stdlib.h>
 
 

int init_list(int_ll_t *list) {
    list->head = NULL;
    if (pthread_mutex_init(&list->mutex, NULL) != 0) {
        return -1;
    }
    return 0;
}

// Free list structure
int free_list(int_ll_t *list) {
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    pthread_mutex_unlock(&list->mutex);
    pthread_mutex_destroy(&list->mutex);
    return 0;
}

int size_list(int_ll_t *list) {
    return list->size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value) {
    pthread_mutex_lock(&list->mutex);
    int count = 0;
    node_t *current = list->head;
    while (current != NULL) {
        if (count == index) {
            *out_value = current->value;
            // printf("count %i\n", out_value);
            pthread_mutex_unlock(&list->mutex);
            return 0; 
        }
        count++;
        current = current->next;
    }
    pthread_mutex_unlock(&list->mutex);
    return -1; 
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value) {
    pthread_mutex_lock(&list->mutex);
    node_t *new_node = malloc(sizeof(node_t));
     
    new_node->value = value;
    if (index >= list->size) {
        index = list->size - 1;
    }
    if (index <= 0) {
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
int remove_list(int_ll_t *list, int index, int *out_value) {
    pthread_mutex_lock(&list->mutex);
    if (list->head == NULL) {
        pthread_mutex_unlock(&list->mutex);
        return -1;
    }
    node_t *current = list->head;
   
    if (index == 0) {
        *out_value = current->value;
        list->head = current->next;
        free(current);
    } else {
        for (int i = 0; i < index - 1; i++) { // con eliminar cualquiera distinto del inicio basta
            if (current->next == NULL) {
                pthread_mutex_unlock(&list->mutex);
                return -1; 
            }
            current = current->next;
        }
        node_t *to_delete = current->next;
        if (to_delete == NULL) {
            pthread_mutex_unlock(&list->mutex);
            return -1;
        }
        *out_value = to_delete->value;
        current->next = to_delete->next;
        free(to_delete);
    }
    list->size--;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}