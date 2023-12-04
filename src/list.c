#include "list.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// Auxiliar Methods
int fix_index(int index, int len)
{
    if (index < 0) index = 0;
    else if (index >= len) index = len - 1;
    return index;
}

// Init list structure
int init_list(int_ll_t *list)
{
    list->root = NULL; 
    list->len = 0; 
    pthread_mutex_init(&list->lock, NULL); 
    return 0;
}
node_t* getNode(int index, int_ll_t* list)
{
    if(index < 0 || index > list->len-1) return NULL;

    node_t* node = list->root;
    int i = 0;
    while (i < index)
    {
        node = node->next;
        i++;
    }
    return node;
}


// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    
    node_t *current = list->root; 
    node_t *next;
    
    while (current != NULL)
    { 
        next = current->next; 
        free(current); 
        current = next;
    }

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    pthread_mutex_unlock(&list->lock);
    return list->len;;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
   
    index = fix_index(index, list->len); 
    node_t* node = getNode(index, list);
    *out_value = node->value;

    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);

    index = fix_index(index, list->len);

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = value;
    
    if (index <= 0)
    { 
        new_node->next = list->root;
        list->root = new_node;
    } 
    else 
    {
        node_t* newPrev = getNode(index-1, list);
        node_t* newNext = newPrev->next;
        
        new_node->next = newNext;
        new_node->prev = newPrev;
        
        newNext->prev = new_node;
        newPrev->next = new_node;
    }

    list->len++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    
    index = fix_index(index, list->len);
    node_t *current_node = list->root;

    // Deleting in empty list
    if (current_node == NULL)
    { 
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    // Deleting root
    if (index == 0)
    {        
        list->root = list->root->next;
    } 
    // Deleting other node
    else 
    {
        node_t *previous = getNode(index-1, list);
        current_node = getNode(index, list);
        previous->next = current_node->next;
    }

    *out_value = current_node->value;
    free(current_node); 

    list->len--; 
    pthread_mutex_unlock(&list->lock);
    return 0;
}
