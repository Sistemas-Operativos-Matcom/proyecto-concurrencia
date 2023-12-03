#include <stdlib.h>
#include "list.h"

// Init list structure
int init_list(int_ll_t *list) {
    list->head = NULL;
    list->len = malloc(sizeof(int));

    if (list->len == NULL) {
        return -1; // Error en asignación de memoria
    }
    list->len = 0;
    return pthread_mutex_init(&list->lock, NULL);
}

int free_list(int_ll_t *list) {
    if (list == NULL) {
        return -1; // null check
    }
    
    int_node *current = list->head;
    while (current != NULL) {
        int_node *next = current->next;
        free(current);
        current = next;
    }
    if (list->len != NULL) {
        list->len = NULL; // to avoid reference problems
    }
    pthread_mutex_destroy(&list->lock);
    free(list);

    return 0; // Éxito
}

int size_list(int_ll_t *list) {
    if (list == NULL) {
        return -1; 
    }
    int lock = pthread_mutex_lock(&list->lock); // block
    int length = (list->len); // get the length
    pthread_mutex_unlock(&list->lock); // unlock
    
    return length;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value) {
    if (list == NULL || out_value == NULL) {
        return -1; 
    }
    
    int lock = pthread_mutex_lock(&list->lock); // Block
    if (list->len == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }
    if (list->head == NULL || index < 0) {
        index = 0;
    } 
    if (index >= list->len)
    {
        index = list->len -1;
    }
     
    int_node *current = list->head;
    int i = 0;
    // explore the linked list
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }

    *out_value = current->data;    
    
    pthread_mutex_unlock(&list->lock); 
    
    return 0; 
}

// Inserta un elemento en un índice específico de la lista enlazada
int insert_list(int_ll_t *list, int index, int value) {
    
    int lockResult = pthread_mutex_lock(&(list->lock));

    if (index > list->len)
        index = list->len;
    if (index < 0)
        index = 0;

    int_node *new_node = malloc(sizeof(int_node));
    if (new_node == NULL) {

        pthread_mutex_unlock(&(list->lock));
        return -1; // Indicar un error
    }
    new_node->data = value;
    
    if (index == 0) {
 
        new_node->next = list->head;
        list->head = new_node;
    } else {
 
        int_node *current_node = list->head;
        for (size_t i = 0; i < index - 1; i++) {
            current_node = current_node->next;
        }
 
        new_node->next = current_node->next;
        current_node->next = new_node;
    }
     
    list->len++;
    pthread_mutex_unlock(&(list->lock));
    return 0; 
}


int remove_list(int_ll_t *list, int index, int *out_value) {
    int lockResult = pthread_mutex_lock(&(list->lock));
    

    if (list->len == 0 || index < 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1; 
    }

    if (index >= list->len)
        index = list->len - 1;

    int_node *node_to_remove = NULL;

    if (index == 0) {
    
        node_to_remove = list->head;
        list->head = node_to_remove->next;
    } else {
    
        int_node *current_node = list->head;
        for (size_t i = 0; i < index - 1; i++) {
            current_node = current_node->next;
        }
        node_to_remove = current_node->next;
        current_node->next = node_to_remove->next;
    }

    
    *out_value = node_to_remove->data;

    
    free(node_to_remove);
    list->len--;

    pthread_mutex_unlock(&(list->lock));
    return 0;
}
