#include <stdio.h>
#include "list.h"

sem_t m;

// Init list structure
int init_list(int_ll_t *list)
{
    
    sem_init(&m, 0, 1); 
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    
    if(NULL != list->next ){

        free_list(list->next);
        
        
    }else{

        free(list);
        list = NULL;

    }

        

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    sem_wait(&m);
    int_ll_t *temp = list;

    int elements = 0;

    while(NULL != temp){

        elements++;
        temp = temp->next;

    }
    sem_post(&m);
    return elements;
}


// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{

    int_ll_t *elem = list;
    if(NULL == elem)return 1;

    if(index <= 0){

        *out_value = list->value;

    }

    for(int c = 0 ; c < index;c++){

        if(NULL != elem->next){

            elem = list->next;

        }else{

            break;

        }

    }

    *out_value = elem->value;

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{

    sem_wait(&m);
    int_ll_t *temp = list;
    if( NULL == temp ){

        temp = (int_ll_t *) malloc(sizeof(int_ll_t));
        temp->value = 0;
        temp->next = NULL;

    }
    
    if(index <= 0)temp->value = value;

    for(int c = 0; c < index;c++){

        if(NULL == temp->next){

            temp->next = (int_ll_t *) malloc(sizeof(int_ll_t));
            temp = temp->next;
            temp->value = 0;
            temp->next = NULL;

        }else{

            temp = temp->next;

        }

    }

    temp->value = value;
    sem_post(&m);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    int_ll_t *elem = list;
    if(NULL == elem)return 1;

    if(index <= 0){

        *out_value = list->value;
        list->value = 0;
    }

    for(int c = 0 ; c < index;c++){

        if(NULL != elem->next){

            elem = list->next;

        }else{

            break;

        }

    }

    *out_value = elem->value;
    elem->value = 0;

    return 0;
}
