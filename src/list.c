#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    // printf("Lista_inicializada \n");
    // fflush(stdout);
    list->head = NULL;
    list->len = 0;
    return pthread_mutex_init(&(list->mutex), NULL);
}

// Free list structure
int free_list(int_ll_t *list)
{
    // Lock the mutex
    pthread_mutex_lock(&(list->mutex));

    // Free the nodes in the linked list
    node_t *n = list->head;
    node_t *next = NULL;
    while (n != NULL)
    {
        next = n->next;
        free(n);
        n = next;
    }

    // Unlock the mutex
    pthread_mutex_unlock(&(list->mutex));
    int destroy = pthread_mutex_destroy(&(list->mutex));
    free(list);
    return destroy;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    if (list->len == 0)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    else
    {
        pthread_mutex_unlock(&(list->mutex));
        return list->len;
    }
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{

    // Bloquear el mutex
    pthread_mutex_lock(&(list->mutex));
    // printf("INDEX METODO //////////////////////////////");
    // fflush(stdout);
    if (list->len == 0)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    // Recorrer la linked list hasta encontrar el elemento
    node_t *n = list->head;
    if (index <= 0 || list->len == 1)
    {
        *out_value = n->data;
    }
    else if (index >= list->len - 1)
    {
        for (int i = 0; i < list->len - 1; i++)
        {
            // printf("Indexando: \n");
            // printf("index_node: %d - value_node: %d . \n", i, n->data);
            // fflush(stdout);
            if (n->next == NULL && i < list->len - 1)
            {
                printf("error en index");
                exit(1);
            }
            n = n->next;
        }
        *out_value = n->data;
    }
    else
    {
        for (int i = 0; i < index; i++)
        {
            // printf("Indexando: \n");
            // printf("index_node: %d - value_node: %d . \n", i, n->data);
            // fflush(stdout);
            if (n == NULL)
            {
                printf("error en index");
                exit(1);
            }
            n = n->next;
        }

        *out_value = n->data;
    }
    // Desbloquear el mutex
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // Bloquear el mutex
    pthread_mutex_lock(&(list->mutex));
    // printf("///////////////////INDEX INSERT: %d\n", index);
    // fflush(stdout);
    node_t *new_node = malloc(sizeof(node_t));
    new_node->data = value;
    new_node->next = NULL;

    if (index <= 0 || list->len == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
        list->len++;
    }
    else if (index >= list->len - 1)
    {
        node_t *last_node = list->head;

        for (int i = 0; i < list->len - 1; i++)
        {
            // printf("iNSERTANDO: \n");
            // printf("index_node: %d - value_node: %d . \n", i, last_node->data);
            // fflush(stdout);
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
            // printf("iNSERTANDO: \n");
            // printf("index_node: %d - value_node: %d . \n", i, temp->data);
            // fflush(stdout);
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
        list->len++;
    }
    // print all aelement in list
    // printf("ACTUAL LIST.Len = %d \n", list->len);
    // printf("ELEMENTS IN LIST \n");
    // node_t *x = malloc(sizeof(node_t));
    // x = list->head;
    // for (int i = 0; i < list->len; i++)
    // {
    //     printf("index: %d - value: %d . \n", i, x->data);
    //     fflush(stdout);
    //     x = x->next;
    // }
    // Desbloquear el mutex
    pthread_mutex_unlock(&(list->mutex));

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{

    // Bloquear el mutex
    pthread_mutex_lock(&(list->mutex));
    // printf("ENTRO A REMOVE ////////////////////////////");
    // fflush(stdout);
    if (list->len == 0)
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    // Buscar el nodo que contiene el dato y el nodo anterior
    node_t *n = list->head;
    node_t *prev = NULL;

    if (index <= 0 || list->len == 1)
    {
        // printf("eliminando primer elemento");
        // fflush(stdout);

        *out_value = n->data;
        list->head = n->next;
        free(n);

        list->len--;
    }
    else if (index >= list->len - 1)
    {
        // printf("eliminando ultimo elemento");
        // fflush(stdout);

        // searching for node before the last one
        for (int i = 0; i < list->len - 2; i++)
        {
            // printf("Eliminando: \n");
            // printf("index_node: %d - value_node: %d . \n", i, n->data);
            if (n->next == NULL && i < list->len - 1)
            {
                printf("error en remove");
                exit(1);
            }
            n = n->next;
        }
        *out_value = n->next->data;
        n->next = NULL;
        free(n->next);
        list->len--;
    }
    else
    {
        // printf("eliminando elemento en index %d \n", index);
        for (int i = 0; i < index; i++)
        {
            // printf("Eliminando: \n");
            // printf("index_node: %d - value_node: %d . \n", i, n->data);
            if (n == NULL)
            {
                printf("error en remove");
                exit(1);
            }
            prev = n;
            n = n->next;
        }
        prev->next = n->next;
        *out_value = n->data;
        free(n);
        list->len--;
    }

    // printf("ELEMENTS IN LIST \n");
    // node_t *x = malloc(sizeof(node_t));
    // x = list->head;
    // for (int i = 0; i < list->len; i++)
    // {
    //     printf("index: %d - value: %d . \n", i, x->data);
    //     fflush(stdout);
    //     x = x->next;
    // }

    // printf("ACTUAL LIST.Len = %d \n", list->len);
    //  Desbloquear el mutex
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}
