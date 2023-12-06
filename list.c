#include <stdlib.h>
#include "list.h"
#include <pthread.h>
  
// Init list structure
int init_list(int_ll_t *list)
{
    list->root = NULL;
    list->size = 0;
    list->last = NULL;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    Node *u = list->root;
    for (int i = 0; i < list->size; i++)
    {
        Node *v = u;
        u = u->next;
        free(v);
    }
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
} 
 
// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    int size = list->size;
    pthread_mutex_unlock(&list->lock);
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if(list->size == 0) 
    {
        pthread_mutex_unlock(&list->lock);          
        return 1;
    }    
    else if(index <= 0) *out_value = list->root->value;
    else if(index >= list->size - 1) *out_value = list->last->value;
    else
    {
        Node *u = list->root;
        for (int i = 0; i <= index; i++)
        {
            if(i == index) *out_value = u->value;
            u = u->next;
        }
    }    
    pthread_mutex_unlock(&list->lock);  
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        pthread_mutex_unlock(&list->lock);
        return 1; 
    }
    node->value = value;
    node->next = NULL;

    if(index < 0) index = 0;
    else if(index > list->size - 1) index = list->size;

    Node *current = list->root;
    Node *prev = NULL;
    for (int i = 0; i < index; i++)
    {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) 
    {
        node->next = list->root;
        list->root = node;
        if(list->size == 0) list->last = node;
    }
    else 
    {
        node->next = prev->next;
        prev->next = node;
    } 

    if(index == list->size)
    {
        list->last->next = node;
        list->last = node;
    }

    list->size += 1;   
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if(list->size == 0) 
    {
        pthread_mutex_unlock(&list->lock);          
        return 1;
    }
    if(index < 0) index = 0;
    else if(index > list->size - 1) index = list->size -1;

    Node *current = list->root;
    Node *prev = NULL;
    for (int i = 0; i < index; i++)
    {
        prev = current;
        current = current->next;
    }

    if(prev == NULL) list->root = current->next;
    else prev->next = current->next;
    if(index >= list->size - 1) list->last = prev;
    *out_value = current->value;
    free(current);
    list->size -= 1;
    
    pthread_mutex_unlock(&list->lock);
    return 0;
}
