#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&(list->lock), NULL); 
    list->head = NULL;            
    list->size = 0;                 
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);

    node_t *cur = list->head;
    while (cur != NULL) {
        node_t *temp = cur->next;
        free(cur);                 
        cur = temp;
    }

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->size; 
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));

    if (list->size == 0) {
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }

    index = max(index, 0);
    index = min(index, list->size-1);

    node_t* node = list->head;

    while (index --) {
        node = node->next;
    }

    *out_value = node->value;

    pthread_mutex_unlock(&(list->lock));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);

    index = max(index, 0);
    index = min(index, list->size);

    node_t* new = (node_t*) malloc(sizeof(node_t)); 
    new->value=value;
    new->next=NULL;
    
    if(index==0){
        new->next = list->head;
        list->head = new;
        list->size ++;
    }
    else{
        node_t* cur = list->head;

        while (--index > 0) {           // ojo ojo OJO
            cur = cur->next;
        }
        
        node_t* temp=cur->next;
        cur->next=new;
        new->next=temp;

        list->size ++;
    }
    
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->lock));

    if(list->size==0){
        pthread_mutex_unlock(&(list->lock));
        return 1;
    }

    index = max(index, 0);
    index = min(index, list->size-1);

    if (index == 0)
    {
        node_t *temp = list->head;
        list->head = list->head->next;
        *out_value = temp->value;
        free(temp);
    }
    else
    {
        node_t* cur = list->head;

        while (--index > 0) {           // ojo ojo OJO
            cur = cur->next;
        }

        *out_value=cur->next->value;

        if(cur->next->next==NULL){
            free(cur->next);
            cur->next = NULL;
        }
        else{
            node_t* temp=cur->next->next;
            free(cur->next);
            cur->next=temp;
            // cur->next=cur->next->next;
        }
    }

    list->size--;

    pthread_mutex_unlock(&(list->lock));
    return 0;
}
