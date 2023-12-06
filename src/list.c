#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&list->lock, NULL);
    list->head=NULL;
    list->size=0;
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); 
    //If there is not elements in list, then free the list
    if(list->size==0){
        free(list);
        pthread_mutex_unlock(&list->lock); 
        pthread_mutex_destroy(&list->lock);
        return 0;
    }
    
    //If there is elements in list, free all the elements and free the list
    node_t* beggin=list->head;
    while (beggin!=NULL)
    {
        node_t* next=beggin->next;
        free(beggin);
        beggin=next;
    }
    free(list);
    pthread_mutex_unlock(&list->lock); 
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock); 
    int to_ret= list->size;
    pthread_mutex_unlock(&list->lock); 
    return to_ret;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    
    pthread_mutex_lock(&list->lock); 
    if(list->size==0) {
      pthread_mutex_unlock(&list->lock);
      return 1;
    }
    //Fix index
    int ind=index;
    if(index<0)ind =0;
    if(index >=list->size) ind=list->size-1;
    //Search and return the right value 
    int n = 0;
    node_t* search_index=list->head;
    while (search_index!=NULL)
    {
        if(n==ind){
            *out_value=search_index->value;
            pthread_mutex_unlock(&list->lock); 
            return 0;
        }
        n++;
        search_index=search_index->next;
    }
    pthread_mutex_unlock(&list->lock); 
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    
    pthread_mutex_lock(&list->lock); 
    //If list is empty, create the new node and set the node as head
    if(list->size==0){
      list->head=(node_t*)malloc(sizeof(node_t));
      list->head->value=value;
      list->head->prev=NULL;
      list->head->next=NULL;
      list->size=1;
      pthread_mutex_unlock(&list->lock); 
      return 0;
    }
    
    //If list is not empty, insert the new node
    //Fix index
    int ind=index;
    if(index<0)ind =0;
    if(index >=list->size) ind=list->size-1;
    int n = 0;
    node_t* search_index=list->head;
    while (search_index!=NULL)
    {
        if(n==ind){
            node_t* new_value=(node_t*)malloc(sizeof(node_t));
            new_value->value=value;
            new_value->prev=search_index->prev;
            if(new_value->prev!=NULL) new_value->prev->next=new_value;
            new_value->next=search_index;
            search_index->prev=new_value;
            if(n==0){
                list->head=new_value;
            }
            list->size+=1;
            pthread_mutex_unlock(&list->lock); 
            return 0;
        }
        n++;
        search_index=search_index->next;
    }
    pthread_mutex_unlock(&list->lock); 
    return 0;
    
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    
    pthread_mutex_lock(&list->lock); 
    //If can`t remove anything, finish
    if(list->size==0){
        pthread_mutex_unlock(&list->lock); 
        return 1;
    } 
    //Fix index
    int ind=index;
    if(index<0)ind =0;
    if(index >=list->size) ind=list->size-1;
    int n = 0;

    //Remove the node
    node_t* search_index=list->head;
    while (search_index!=NULL)
    {
        if(n==ind){
          if(search_index->prev != NULL){
            (search_index->prev)->next= search_index->next;
          }
          if(search_index->next != NULL){
            (search_index->next)->prev= search_index->prev;
          }
          if(search_index == list->head){
            list->head=search_index->next;
          }
          *out_value=search_index->value;
          free(search_index);
          list->size-=1;
          pthread_mutex_unlock(&list->lock); 
          return 0;
        }
        n++;
        search_index=search_index->next;
    }
    pthread_mutex_unlock(&list->lock); 
    return 0;
}

