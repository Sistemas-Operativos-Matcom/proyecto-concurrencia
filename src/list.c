#include "list.h"
#include <stdlib.h>
#include <pthread.h>

// Inicializa la estructura de la lista
int init_list(int_ll_t *list) {
    list = (int_ll_t *)malloc(sizeof(int_ll_t));
    if (list == NULL) {
        // Manejo de error al asignar memoria
        return -1;
    }

    list->head = NULL;
    list->size = 0;
    
    int result = pthread_mutex_init(&(list->mutex), NULL);
    if (result != 0) {
        // Manejo de error al inicializar el mutex
        free(list);
        return result;
    }

    return 0;  // Éxito
}

// Libera la estructura de la lista
int free_list(int_ll_t *list) {
    if (list == NULL) {
        return -1;  // Lista no inicializada
    }

    pthread_mutex_lock(&(list->mutex));
    free(list);
    pthread_mutex_unlock(&(list->mutex));

    return pthread_mutex_destroy(&(list->mutex));
}

// Obtiene el tamaño de la lista
int size_list(int_ll_t *list) {
    if (list == NULL) {
        return -1;  // Lista no inicializada
    }

    pthread_mutex_lock(&(list->mutex));
    int size = list->size;
    pthread_mutex_unlock(&(list->mutex));

    return size;
}

// Obtiene el elemento en el índice dado
int index_list(int_ll_t *list, int index, int *out_value) {
    if (list == NULL || index < 0) {
        return -1;  // Lista no inicializada o índice inválido
    }

    pthread_mutex_lock(&(list->mutex));

    if (list->size == 0 || index >= list->size) {
        pthread_mutex_unlock(&(list->mutex));
        return -1;  // Índice fuera de rango
    }

    ll_node_t *current_node = list->head;
    for (int i = 0; i < index; i++) {
        current_node = current_node->next_node;
    }

    *out_value = current_node->value;
    pthread_mutex_unlock(&(list->mutex));

    return 0;  // Éxito
}

// Inserta un elemento en el índice dado
int insert_list(int_ll_t *list, int index, int value) {
    if (list == NULL || index < 0) {
        return -1;  // Lista no inicializada o índice inválido
    }

    pthread_mutex_lock(&(list->mutex));

    ll_node_t *new_node = (ll_node_t *)malloc(sizeof(ll_node_t));
    if (new_node == NULL) {
        // Manejo de error al asignar memoria
        pthread_mutex_unlock(&(list->mutex));
        return -1;
    }

    new_node->value = value;

    if (index == 0 || list->size == 0) {
        new_node->next_node = list->head;
        list->head = new_node;
    } else if (index < list->size) {
        ll_node_t *current_node = list->head;
        for (int i = 1; i < index; i++) {
            current_node = current_node->next_node;
        }

        new_node->next_node = current_node->next_node;
        current_node->next_node = new_node;
    } else {
        ll_node_t *current_node = list->head;
        while (current_node->next_node != NULL) {
            current_node = current_node->next_node;
        }

        new_node->next_node = NULL;
        current_node->next_node = new_node;
    }

    list->size++;
    pthread_mutex_unlock(&(list->mutex));
    return 0;  // Éxito
}

// Elimina el elemento en el índice dado
int remove_list(int_ll_t *list, int index, int *out_value) {
    if (list == NULL || index < 0) {
        return -1;  // Lista no inicializada o índice inválido
    }

    pthread_mutex_lock(&(list->mutex));

    if (list->size == 0 || index >= list->size) {
        pthread_mutex_unlock(&(list->mutex));
        return -1;  // Índice fuera de rango
    }

    ll_node_t *current_node = list->head;

    if (index == 0 || list->size == 1) {
        *out_value = list->head->value;
        list->head = current_node->next_node;
        free(current_node);
    } else {
        for (int i = 1; i < index; i++) {
            current_node = current_node->next_node;
        }

        ll_node_t *next_node_to_remove = current_node->next_node;
        *out_value = next_node_to_remove->value;
        current_node->next_node = next_node_to_remove->next_node;
        free(next_node_to_remove);
    }

    list->size--;
    pthread_mutex_unlock(&(list->mutex));
    return 0;  // Éxito
}
