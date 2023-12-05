#include "list.h"
#include <stdlib.h>
#include <pthread.h>


static node_t *create_node(int value)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (new_node == NULL)
    {
        return NULL;  
    }

    new_node->value = value;
    new_node->next = NULL;

    pthread_mutex_init(&new_node->lock, NULL);

    return new_node;
}

int init_list(int_ll_t *list) 
{
    list->head = NULL;
    pthread_mutex_init(&list->list_lock, NULL);

    return 0;
}

int free_list(int_ll_t *list) 
{
    pthread_mutex_lock(&list->list_lock);

    node_t *current = list->head;

    while (current != NULL) 
    {
        node_t *temp = current;
        current = current->next;
        pthread_mutex_destroy(&temp->lock);
        free(temp);
    }

    list->head = NULL;
    pthread_mutex_unlock(&list->list_lock);
    pthread_mutex_destroy(&list->list_lock);
    
    return 0;
}

int size_list(int_ll_t *list) 
{
    pthread_mutex_lock(&list->list_lock);

    int size = 0;
    node_t *current = list->head;

    while (current != NULL)
    {
        size++;
        current = current->next;
    }

    pthread_mutex_unlock(&list->list_lock);

    return size;
}

int index_list(int_ll_t *list, int index, int *out_value) 
{
    pthread_mutex_lock(&list->list_lock);
    node_t *current = list->head;

    if (index < 0) 
    {
        index = 0;
    }

    int i;
    for (i = 0; i < index && current != NULL; i++) 
    {
        current = current->next;
    }

    if (current != NULL) 
    {
        *out_value = current->value;
    }

    pthread_mutex_unlock(&list->list_lock);

    return 0;
}

int insert_list(int_ll_t *list, int index, int value) 
{
  pthread_mutex_lock(&list->list_lock);

    if (index < 0) 
    {
        index = 0;
    }

    node_t *new_node = create_node(value);

    if (new_node == NULL) 
    {
        pthread_mutex_unlock(&list->list_lock);
        return 1;  
    }

    
    if (index == 0) 
    {
        new_node->next = list->head;
        list->head = new_node;
    } 

    else 
    {
        
        node_t *current = list->head;
        int i;
        for (i = 0; i < index - 1 && current != NULL; i++) 
        {
            current = current->next;
        }

        if (current != NULL) 
        {
            new_node->next = current->next;
            current->next = new_node;
        }

        else 
        {
            free(new_node);
        }
    }

    pthread_mutex_unlock(&list->list_lock);

    return 0;
}


int remove_list(int_ll_t *list, int index, int *out_value) 
{
    pthread_mutex_lock(&list->list_lock);

    if (index < 0) 
    {
        index = 0;
    }

    if (index == 0) 
    {
        if (list->head != NULL) 
        {
            node_t *temp = list->head;
            *out_value = temp->value;
            list->head = list->head->next;
            pthread_mutex_unlock(&list->list_lock);
            free(temp);

            return 0;
        } 

        else 
        {
            pthread_mutex_unlock(&list->list_lock);

            return 1;
        }
    } 
    
    else 
    {
        node_t *current = list->head;
        int i;

        for (i = 0; i < index - 1 && current != NULL; i++)
        {
            current = current->next;
        }

        if (current != NULL && current->next != NULL) 
        {
            node_t *temp = current->next;
            *out_value = temp->value;
            current->next = temp->next;
            pthread_mutex_unlock(&list->list_lock);
            free(temp);

            return 0;
        } 

        else 
        {
            pthread_mutex_unlock(&list->list_lock);

            return 1;
        }
    }
}
