#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->len = 0;
    pthread_mutex_init(&list->mutex, NULL);
    return 1;
}

// Free list structure
int free_list(int_ll_t *list)
{
    // Lock the mutex
    pthread_mutex_lock(&list->mutex);

    // Free the nodes in the linked list
    node_t *n = list->head;
    node_t *next = NULL;
    while (n != NULL)
    {
        next = n->next;
        free(n);
        n = next;
    }

    free(list);
    // Unlock the mutex
    pthread_mutex_unlock(&list->mutex);
    pthread_mutex_destroy(&(list->mutex));
    return 1;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->len;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // Bloquear el mutex
    pthread_mutex_lock(&list->mutex);

    // Recorrer la linked list hasta encontrar el elemento
    node_t *n = list->head;

    for (int i = 0; i < index; i++)
    {
        if (n == NULL)
        {
            printf("error en index");
            exit(1);
        }
        n = n->next;
    }

    *out_value = n->data;

    // Desbloquear el mutex
    pthread_mutex_unlock(&list->mutex);
    return 1;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // Bloquear el mutex
    pthread_mutex_lock(&list->mutex);

    node_t *new_node = malloc(sizeof(node_t));
    new_node->data = value;
    new_node->next = NULL;

    if (index <= 0)
    {
        new_node->next = list->head;
        list->head = new_node;
        list->len++;
    }
    else if (index >= list->len)
    {
        node_t *last_node = list->head;

        for (int i = 0; i < list->len; i++)
        {
            if (last_node->next == NULL && i < list->len - 1)
            {
                printf("error en insercion");
                exit(1);
            }
            last_node = last_node->next;
        }
        last_node->next = new_node;
        list->len++;
    }
    else
    {
        node_t *temp = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
        list->len++;
    }
    // Desbloquear el mutex
    pthread_mutex_unlock(&list->mutex);
    return 1;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    // Bloquear el mutex
    pthread_mutex_lock(&list->mutex);

    // Buscar el nodo que contiene el dato y el nodo anterior
    node_t *n = list->head;
    node_t *prev = NULL;

    for (int i = 0; i < index; i++)
    {
        if (n == NULL)
        {
            printf("error en remove");
            exit(1);
        }
        prev = n;
        n = n->next;
    }

    // Si se encontró el nodo, eliminarlo de la linked list y liberar la memoria
    if (n != NULL)
    {
        if (prev == NULL)
        {
            list->head = n->next;
        }
        else
        {
            prev->next = n->next;
        }
        free(n);
    }
    // Desbloquear el mutex
    pthread_mutex_unlock(&list->mutex);
    return 1;
}
