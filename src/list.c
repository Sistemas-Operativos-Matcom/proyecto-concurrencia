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
    // TODO: Your code here!
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
    // TODO: Your code here!
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
    // TODO: Your code here!
    return 0;
}
