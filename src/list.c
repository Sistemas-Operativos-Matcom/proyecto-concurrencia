#include "list.h"
#include "pthread.h"
#include "semaphore.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list = (int_ll_t*) malloc(sizeof(int_ll_t));
    list->size = 0;
    list->first = NULL;
    int failed = pthread_mutex_init(&(list->general_mutex), NULL);
    return failed;
}

// Free list structure
int free_list(int_ll_t *list)
{   
    int_ll_node* current = list->first;
    while(current != NULL){
        int_ll_node* tmp = current->next;
        free(current);
        current = tmp;
    }
    int failed = pthread_mutex_destroy(&(list->general_mutex));
    //free(list); //??
    return failed;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->general_mutex));
    int size = list->size;
    if(size == 0) size = 1;
    pthread_mutex_unlock(&(list->general_mutex));
    return size;
}

int_ll_node* helper_get_node_at_index(int_ll_t* list, int index){
    if(list->first == NULL || index < 0){
        return list->first;
    }else{
        int_ll_node* current = list->first;
        int current_index = 0;
        while(current->next != NULL && current_index < index){
            current_index += 1;
            current = current->next;
        }
        return current;
    }
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->general_mutex));

    int_ll_node* node = helper_get_node_at_index(list, index);
    if(node == NULL){
        *out_value = 0;
    }else{
        *out_value = node->value;
    }

    pthread_mutex_unlock(&(list->general_mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->general_mutex));

    if(index <= 0 || list->first == NULL){
        int_ll_node* tmp = list->first;
        list->first = (int_ll_node*)malloc(sizeof(int_ll_node));
        list->first->next = tmp;
        list->first->value = value;
    }else{
        int_ll_node* prev_node = helper_get_node_at_index(list, index-1);
        int_ll_node* tmp = prev_node->next;
        prev_node->next = (int_ll_node*)malloc(sizeof(int_ll_node));
        prev_node->next->next = tmp;
        prev_node->next->value = value;
    }

    list->size += 1;

    pthread_mutex_unlock(&(list->general_mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->general_mutex));
    
    int result = list->first == NULL;
    if(list->first != NULL){
        if(index < 0){
            index = 0;
        }else if(index > list->size-1){
            index = list->size-1;
        }
        
        if(index == 0){
            int_ll_node* tmp = list->first;
            *out_value = tmp->value;
            list->first = list->first->next;
            free(tmp);
        }else{
            int_ll_node* prev_node = helper_get_node_at_index(list, index-1);
            int_ll_node* tmp = prev_node->next;
            *out_value = tmp->value;
            prev_node->next = prev_node->next->next;
            free(tmp);
        }

        list->size -= 1;
    }

    pthread_mutex_unlock(&(list->general_mutex));
    return result;
}
