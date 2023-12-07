#include "list.h"
#include <stdio.h>

// Init list structure
int init_list(int_ll_t *list)
{
    list->len = 0;
    list->node = NULL;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    int* m;
    for(int i = 0; list->node != NULL;){
        node_t* n = list->node;
        list->node = n->next;
        free(n);
    }

    pthread_mutex_destroy(&list->lock);
    free(list);
    
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->len;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    if(list->len == 0){
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if(index < 0) {
        index = 0;
    }

    else if (index >= list->len){
        index = list->len - 1;
    }

    node_t* node = list->node;
    for(int i = 0; i < index; i++) {
        node = node->next;
    }

    *out_value = node->value;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);

    if(index < 0){
        index = 0;
    }

    else if (index > list->len){
        index = list->len;
    }

    node_t* node;
    if(index == 0){
        node = (node_t*)malloc(sizeof(node_t));
        node->value = value;
        node->next = list->node;
        list->node = node;
    }
    
    else{ 
        node_t* node = list->node;
        for(int i = 0; i < index - 1; i++) {
            node = node->next;
        }

        node_t* n = (node_t*)malloc(sizeof(node_t));
        n->value = value;
        n->next = node->next;
        node->next = n;
    }

    list->len++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    if(list->len == 0){
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if(index < 0) {
        index = 0;
    }

    else if (index >= list->len){
        index = list->len - 1;
    }

    node_t* node = list->node;
    if(index == 0){
        *out_value = list->node->value;
        list->node = node->next;
        free(node);
    }

    else{
        for(int i = 0; i < index - 1; i++) {
            node = node->next;
        }
        
        node_t* n = node->next;
        *out_value = n->value;
        node->next = n->next;
        free(n);
    }

    list->len--;
    if (list->len == 0) list->node = NULL;

    pthread_mutex_unlock(&list->lock);
    return 0;
}
