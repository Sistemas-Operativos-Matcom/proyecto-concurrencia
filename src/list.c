#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    // TODO: Your code here!
    list=(int_ll_t*)malloc(sizeof(int_ll_t));
    list->head=NULL;
    list->size=0;
    int ret = pthread_mutex_init(&(list->lock),NULL);
    return ret;
}

// Free list structure
int free_list(int_ll_t *list)
{
    // TODO: Your code here!
    node *curr = list->head;
    node *next;
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    int x=pthread_mutex_destroy(&(list->lock));
    if(!x){free(list);}
    return x;
}

// Get list size
int size_list(int_ll_t *list)
{
    // TODO: Your code here!
    return list->size;
}

// Get node at index
node *get_node_at_index(int_ll_t *list, int index)
{
    if (list->size == 0 || index <= 0)
        return list->head;
    else
    {
        int i = 0;
        node *curr = list->head;
        while (curr->next != NULL && i < index)
        {
            i += 1;
            curr = curr->next;
        }
        return curr;
    }
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // TODO: Your code here!
    pthread_mutex_lock(&(list->lock));
    node *search_result = get_node_at_index(list, index);
    int ret=0;
    if (search_result == NULL){
        ret=1;
    }
    else{
        *out_value = search_result->val;
    }
    pthread_mutex_unlock(&(list->lock));
    return ret;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->lock));
    node *new_node = (node *)malloc(sizeof(node));
    new_node->val = value;
    if (list->size==0|| index <= 0){
        node *prev_head = list->head;
        list->head = new_node;
        list->head->next = prev_head;
    }
    else{
        node *prev = get_node_at_index(list, index - 1);
        node *new_next = prev->next;
        prev->next = new_node;
        new_node->next = new_next;
    }
    list->size++;
    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));
    int ret=0;
    if (list->size != 0){
        list->size--;
        if (index <= 0){
            node *to_del = list->head;
            *out_value = to_del->val;
            list->head = to_del->next;
            free(to_del);
        }
        else
        {
            node *prev = get_node_at_index(list, index - 1);
            if(prev->next == NULL && index > 0){
                index = list->size-1;
                prev = get_node_at_index(list, index - 1);
            }
            if (index <= 0){
                node *to_del = list->head;
                *out_value = to_del->val;
                list->head = to_del->next;
                free(to_del);
            }
            else{
                node *to_del = prev->next;
                *out_value = to_del->val;
                prev->next = to_del->next;
                free(to_del);
            }
        }
    }
    else{
        ret=1;
    }
    pthread_mutex_unlock(&(list->lock));
    return ret;
}

