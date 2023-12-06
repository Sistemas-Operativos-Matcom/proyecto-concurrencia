#include <stdio.h>
#include "list.h"
sem_t m;
int elements = 0;

// Init list structure
int init_list(int_ll_t *list)
{
    sem_init(&m, 0, 1); 

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    
    sem_wait(&m);
    if(NULL != list->next ){
        free_list(list->next);
        
        
    }else{

        free(list);
        list = NULL;

    }

        
    sem_wait(&m);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return elements;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    sem_wait(&m);
    if(list == NULL){
        sem_post(&m);
        return 1;}

    if(index <= 0){
        *out_value = list->value;
        sem_post(&m);
        return 0;
    }
    int_ll_t *temp = list;
    
    if(index > elements)index = elements;

    for(int c = 0;c < index;c++){

        if(temp->next != NULL){
            temp = temp->next;
        }else{

            *out_value = temp->value;
            sem_post(&m);
            return 0;

        }

    }
    *out_value = temp->value;
    sem_post(&m);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{

    sem_wait(&m);
    elements++;
    int_ll_t *temp = list;
    int_ll_t *prev = temp;

    for(int c = 0; c < index;c++){

        if(NULL == temp->next){
            temp->next = (int_ll_t *) malloc(sizeof(int_ll_t));
            temp = temp->next;
            temp->value = value;
            temp->next = NULL;
            sem_post(&m);
            return 0;

        }else{
            prev = temp;
            temp = temp->next;
        }

    }

    if(index <= 0){
       
        temp = (int_ll_t *) malloc(sizeof(int_ll_t));
        temp->value = value;
        temp->next = list;
        list = temp;
    }else{
    
        int_ll_t *new = (int_ll_t *) malloc(sizeof(int_ll_t));
        new->value = value;
        new->next = temp;
        prev->next = new;
    }
    sem_post(&m);
    return 0;

}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    sem_wait(&m);
    elements--;
    int_ll_t *elem = list;
    int_ll_t *prev = elem;
    if(NULL == elem)return 1;

    if(index <= 0){

        *out_value = list->value;
        elem = list->next;
        free(list);
        list = elem;
    }

    for(int c = 0 ; c < index;c++){

        if(NULL != elem->next){
            prev = elem;
            elem = elem->next;

        }else{
            *out_value = elem->value;
            if(prev == elem){
                free(list);
                list == NULL;
                sem_post(&m);
                return 0;
            }else{

                prev->next = NULL;
                free(elem);
                sem_post(&m);
                return 0;
            }

        }

    }

    *out_value = elem->value;

    prev->next = elem->next;
    free(elem);

    sem_post(&m);
    return 0;
}
