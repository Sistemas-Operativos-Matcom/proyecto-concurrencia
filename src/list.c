#include "list.h"
#include <stdlib.h>
#include <stdio.h>

// Get a node on x position in a list
Node *Get_Node(int_ll_t *list, int position)
{
    if (list->head == NULL || position <= 0)
    {
        return list->head;
    }
    else
    {
        int i = 0;
        Node *node = list->head;
        while (node->next != NULL && i < position)
        {
            node = node->next;
            i += 1;
        }
        return node;
    }
}

// Init list structure
int init_list(int_ll_t *list)
{
    // Inicializando lista
    list = (int_ll_t *)malloc(sizeof(int_ll_t));
    list->head = NULL;
    list->length = 0;
    // Inicializando mutex
    int success = pthread_mutex_init(&(list->mutex), NULL);
    return success;
}

// Free list structure
int free_list(int_ll_t *list)
{
    // Liberando cada nodo de la lista
    Node *node = list->head;
    while (node != NULL)
    {
        Node *next = node->next;
        free(node);
        node = next;
    }
    // Luego, destruir el mutex y free a la lista
    int success = pthread_mutex_destroy(&(list->mutex));
    if (!success)
    {
        free(list);
    }
    return success;
}

// Get list size
int size_list(int_ll_t *list)
{
    // Bloqueando acceso desde otro hilo
    pthread_mutex_lock(&(list->mutex));
    // Obteniendo tamaño
    int length = list->length;
    // Desbloqueando acceso a otros hilos y devolviendo valor
    pthread_mutex_unlock(&(list->mutex));
    return length;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // Bloqueando acceso desde otro hilo
    pthread_mutex_lock(&(list->mutex));
    // Buscando nodo a devolver
    Node *node = Get_Node(list, index);
    // Verificando que el nodo es válido
    if (node == NULL)
    {
        // Desbloqueando acceso a otros hilos y retornando error
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    // Retornando valor
    *out_value = node->value;
    // Desbloqueando acceso a otros hilos
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // Bloqueando acceso desde otro hilo
    pthread_mutex_lock(&(list->mutex));
    // Creando nuevo nodo con el dato correspondiente
    if (list->head == NULL || index <= 0)
    {
        Node *next = list->head;
        list->head = (Node *)malloc(sizeof(Node));
        list->head->value = value;
        list->head->next = next;
    }
    else
    {
        Node *previous = Get_Node(list, index - 1);
        Node *next = previous->next;
        previous->next = (Node *)malloc(sizeof(Node));
        previous->next->value = value;
        previous->next->next = next;
    }
    // Aumentando el tamaño de lista
    list->length++;
    // Desbloqueando acceso a otros hilos
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    // Bloqueando acceso desde otro hilo
    pthread_mutex_lock(&(list->mutex));
    // Verificando que la lista sea válida
    if (list->head != NULL)
    {
        if (index <= 0)
        {
            Node *delete_node = list->head;
            list->head = list->head->next;
            *out_value = delete_node->value;   
            free(delete_node);
        }
        else
        {
            //Buscando nodo anterior
            Node *previous = Get_Node(list, index - 1);
            while (previous->next == NULL && index > 0)
            {
                index -= 1;
                previous = Get_Node(list, index - 1);
            }
            //Buscando nodo a liberar y liberando nodo
            if (index <= 0)
            {
                Node *delete_node = list->head;
                *out_value = delete_node->value;
                list->head = list->head->next;
                free(delete_node);
            }
            else
            {
                Node *delete_node = previous->next;
                *out_value = delete_node->value;
                previous->next = previous->next->next;
                free(delete_node);
            }
        }
        // Reajustando el tamaño de la lista
        list->length--;
    }
    // Devolviendo error de lista no válida
    else
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;
    }
    // Permitiendo acceso desde los demás hilos y retornando
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}
