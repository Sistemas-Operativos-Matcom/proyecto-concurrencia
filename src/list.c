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
    // TODO: Your code here!
    //free(list);
    pthread_mutex_lock(&(list->mutex));
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
    //outvalue sera distinto de 0 cuando deje de haber un siguiente nodo o alcance el indice
    pthread_mutex_lock(&(list->mutex));
    out_value = 0;
    if (list->length != 0)
    {
        node* node = getNode(list,index);
        out_value = node->value;
    }
  
    // if (index < list->length)
    // {
    //     if (index <= 0)
    //     {
    //         out_value = list->first_node->value;
    //         pthread_mutex_unlock(&(list->mutex));
    //         return 0;
    //     }
        
    //     tuple *prev_node = getNode(list, index);
    //     if (prev_node->index == index)
    //     {
    //         out_value = prev_node->node->value;
    //     }
    // }
    // else
    // {
    //     out_value = list->last_node->value;
    // }

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
        // Inserta el nuevo nodo después del nodo actual
        node* prev_node = get_node(list,index-1);
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



    // node *new_node = (node *)malloc(sizeof(node));
    // new_node->value = value;

    // if (index <= 0)
    // {
    //     list->first_node->prev_node = new_node;
    //     new_node->prev_node = NULL;
    //     new_node->next_node = list->first_node;
    //     list->first_node = new_node;
    // }
    // if (list->length == 0)
    // {
    //     list->first_node = list->last_node = new_node;
    //     new_node->next_node = NULL;
    //     new_node->prev_node = NULL;
    //     list->length ++;
    //     pthread_mutex_unlock(&(list->mutex));
    //     return 0;
    // }
    
    // tuple* adjacent_node = getNode(list, index);
    // if (adjacent_node->index == index)
    // {
    //     adjacent_node->node->value = value;
    //     pthread_mutex_unlock(&(list->mutex));
    //     return 0;
    // }else if (adjacent_node->index < index)
    // {
    //     node* next_node = prev_node->node->next_node;
    //     next_node->prev_node = new_node;
    //     new_node->next_node = next_node;
    //     new_node->prev_node = adjacent_node->node;
    //     adjacent_node->node->next_node = new_node;
    // }
    // else
    // {
    //     node* prev_node = adjacent_node->node->prev_node;
    //     prev_node->next_node = new_node;
    //     new_node->prev_node = prev_node;
    //     new_node->next_node = adjacent_node->node;
    //     adjacent_node->node->prev_node = new_node;
    // }
    // list->length ++;
   
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
            out_value = list->first_node->value;
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
    
    // if (list->length == 0)
    // {
    //     return 1;
    // }
    // if (index <= 0)
    // {
    //     out_value = list->first_node->value;
    //     list->first_node->next_node->prev_node = NULL;
    //     node* temp = list->first_node->next_node;
    //     free(list->first_node);
    //     list->first_node = temp;
        
    // }
    // if (index >= list->length - 1)
    // {
    //     out_value = list->last_node->value;
    //     list->last_node->prev_node->next_node = NULL;
    //     node* temp = list->last_node->prev_node;
    //     free(list->last_node);
    //     list->last_node = temp;
        
    // }else
    // {
    //      tuple* curr_node = getNode(list, index);
    // if (curr_node->index == index)
    // {
    //     out_value = curr_node->node->value;
    //     node* prev_node = curr_node->node->prev_node;
    //     node* next_node = curr_node->node->next_node;
    //     prev_node->next_node = curr_node->node->next_node;
    //     next_node->prev_node = curr_node->node->prev_node;
    //     free(curr_node->node);
    //     list->length --;
    // }
    // free(curr_node);
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
    


    // tuple* answer = (tuple*)malloc(sizeof(tuple));
    // // TODO: Your code here!
    // node *curr_node = list->first_node;
    // int curr_index = 0;
    // int size = list->length;
    // if (index < (size/2))
    // {
    //     while (curr_node->next_node != NULL && curr_index < index)
    //     {
    //         curr_node = curr_node->next_node;
    //         curr_index++;
    //     }
    // }
    // else
    // {
    //     curr_index = list->length;
    //     curr_node = list->last_node;
    //     while (curr_node->prev_node != NULL && curr_index > index)
    //     {
    //         curr_node = curr_node->prev_node;
    //         curr_index--;
    //     }
    // }
    // answer->node = curr_node;
    // answer->index = curr_index;
    // return answer;
}
