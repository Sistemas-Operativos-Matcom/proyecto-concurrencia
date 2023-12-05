#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&list->mutex, NULL);
    list->head = NULL;
    list->size = 0;
    return 0;
}
// Free list structure
int free_list(int_ll_t *list)
{
    node *current = list->head;
    while (current != NULL)
    {
        node *temp = current->next;
        free(current);
        current = temp;
    }
    pthread_mutex_destroy(&(list->mutex));
    free(list);
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

    if (list == NULL || list->size == 0)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    if (index <= 0)
    {
        *out_value = list->head->data;
        pthread_mutex_unlock(&list->mutex);
        return 0;
    }

    int i = 0;
    node *current = list->head;
    while (current->next != NULL)
    {
        if (i == index)
        {
            *out_value = current->data;
            pthread_mutex_unlock(&list->mutex);
            return 0;
        }
        current = current->next;
        i++;
    }

    *out_value = current->data;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->mutex);

    if (list == NULL)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    node *new_node = (node *)malloc(sizeof(node));
    new_node->data = value;

    if (index <= 0 || list->head == NULL)
    {
        new_node->next = list->head;
        list->head = new_node;
        list->size++;
        pthread_mutex_unlock(&list->mutex);
        return 0;
    }

    node *current = list->head;
    if (index > list->size)
        index = list->size; // posible -1.

    for (int i = 0; i < index - 1; i++)
    {
        if (current != NULL)
        {
            current = current->next;
        }
    }
    new_node->next = current->next;
    current->next = new_node;
    list->size++;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->mutex);

    if (list == NULL || list->size == 0)
    {
        pthread_mutex_unlock(&list->mutex);
        return 1;
    }

    node *current = list->head;
    node *previous = NULL;

    if (list->size == 1)
    {
        *out_value = current->data;
        free(current);
        list->head = NULL;
        list->size--;
        pthread_mutex_unlock(&list->mutex);
        return 0;
    }
    

    if (index <= 0)
    {
        *out_value = current->data;
        list->head = current->next;
        free(current);
        list->size--;
        pthread_mutex_unlock(&list->mutex);
        return 0;
    }

    if (index >= list->size-1)
        {
            while (current->next != NULL)
            {
                previous = current;
                current = current->next;
            }
            *out_value = current->data;
            previous->next = NULL;
            free(current);
            list->size--;
            pthread_mutex_unlock(&list->mutex);
            return 0;
        }

    for (int i = 0; i < index - 1; i++)
    {
        if (current != NULL)
        {
            current = current->next;
        }
    }

    node *del = current->next;
    current->next = del->next;
    *out_value = del->data;
    free(del);
    list->size--;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}
