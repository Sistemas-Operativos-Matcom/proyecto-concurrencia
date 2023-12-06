#include "list.h"
#include "semaphore.h"
#include <stdlib.h>
#include <stdio.h>
#include "pthread.h"


// Init list structure
int init_list(int_ll_t *list)
{
    // TODO: Your code here!
    int res = pthread_mutex_init(&(list->mutex), NULL);
    list = (int_ll_t*)malloc(sizeof(int_ll_t));
    list->first_node =  NULL;
    list->length = 0;
    return res;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));
    node* curr_node = list->first_node;
    while(curr_node != NULL){
        node* temp = curr_node->next_node;
        free(curr_node);
        curr_node = temp;
    }
    free(list);
    pthread_mutex_unlock(&(list->mutex));
    int res = pthread_mutex_destroy(&(list->mutex));
    return res;
}

// Get list size
int size_list(int_ll_t *list)
{
    // TODO: Your code here!
    pthread_mutex_lock(&(list->mutex));
    int res = list->length;
    pthread_mutex_unlock(&(list->mutex));
    return res;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // TODO: Your code here!
    
    pthread_mutex_lock(&(list->mutex));
    *out_value = 0;
    if (list->length != 0)
    {
        node* my_node = (node*)malloc(sizeof(node));
        my_node = list->first_node;
        int curr_index = 0;
        while (my_node->next_node != NULL && curr_index < index)
        {
            my_node = my_node->next_node;
            curr_index++; 
        }
        *out_value = my_node->value;
    }

    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));
    node *new_node = (node *)malloc(sizeof(node));
    new_node->value = value;
    if (list->length != 0 && index > 0)
    {
        node* prev_node = (node*)malloc(sizeof(node));
        prev_node = list->first_node;
        int curr_index = 0;
        while (prev_node->next_node != NULL && curr_index < index)
        {
            prev_node = prev_node->next_node;
            curr_index++; 
        }
        node* temp = prev_node->next_node;
        prev_node->next_node = new_node;
        prev_node->next_node->next_node = temp;
    }
    else
    {
            node* temp = list->first_node;
            list->first_node = new_node;
            list->first_node->next_node = temp;       
    }
    list->length ++;
    pthread_mutex_unlock(&(list->mutex));
    return 0;

}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)//se borra ese elemento de la lista
{
    // TODO: Your code here!
    pthread_mutex_lock(&(list->mutex));
    int result = 1;
    if (list->length != 0)
    {
        result = 0;
        if (index <= 0 || list->length == 1)
        {
            *out_value = list->first_node->value;
            node* temp = list->first_node;
            list->first_node = list->first_node->next_node;
            free(temp);
        }
        else{
           node* curr_node = list->first_node;
            int curr_index = 0;
            // Busca el nodo en el índice anterior al que se eliminará
            while (curr_node->next_node != NULL && curr_index < index-1) 
            {
                if (curr_node->next_node->next_node == NULL)
                {
                    break;
                }
                curr_node = curr_node->next_node;
                curr_index++;     
            }
            node* temp = curr_node->next_node;
            *out_value = temp->value;
            curr_node->next_node = curr_node->next_node->next_node;
            free(temp);
        }
        list->length --;
    }
    pthread_mutex_unlock(&(list->mutex));
    return result;
}

node* getNode(int_ll_t *list, int index)
{
    node* answer = (node*)malloc(sizeof(node));
    answer = list->first_node;
    int curr_index = 0;
    while (answer->next_node != NULL && curr_index < index)
    {
        answer = answer->next_node;
        curr_index++; 
    }
    return answer;
}
