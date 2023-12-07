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

int free_list_recursive(int_ll_t *list)
{
    
    if(NULL != list->next ){
        free_list_recursive(list->next);              
    }

        free(list);
        list = NULL;

    return 0;
}
// Free list structure
int free_list(int_ll_t *list)
{
    sem_wait(&m);
        free_list_recursive(list->next);    
    sem_post(&m);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->value;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    
    sem_wait(&m);
    elements = list->value;
    if(list->next == NULL){
        sem_post(&m);
        return 1;
        }

    list = list->next;
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
}//fin de index

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)//segfault sometimes , possible circular reference
{
    sem_wait(&m);
    elements = list->value;
    int_ll_t *temp = list->next;
    int_ll_t *prev = list->next;

    if(index >= elements)index = elements;
    if(index < 0)index = 0;
    list->value++;

    if(index == 0){
        temp = (int_ll_t *) malloc(sizeof(int_ll_t));
        temp->value = value;
        temp->next = list->next;
        list->next = temp;
        sem_post(&m);
        return 0;
    }

    for(int c = 0; c < index;c++){

            prev = temp;
            temp = temp->next;
      
    }

    if(temp == NULL){

        prev->next = (int_ll_t *) malloc(sizeof(int_ll_t));
        temp = prev->next;
        temp->value = value;
        temp->next = NULL;

    }else{

        int_ll_t *new = (int_ll_t *) malloc(sizeof(int_ll_t));
        prev->next = new;
        new->next = temp;
        new->value = value;
        

    }


    sem_post(&m);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)//segfault somewhere here
{
    sem_wait(&m);
    elements = list->value;
    if(index >= elements)index = elements - 1;
    list->value--;
    int_ll_t *elem = list->next;
    int_ll_t *prev = elem;
    if(NULL == elem)return 1;

    if(index <= 0){

        *out_value = elem->value;
        list->next = elem->next;
        elem->next = NULL;


    }else{

        for(int c = 0 ; c < index;c++){

          prev = elem;
          elem = elem->next;
          

         }

        *out_value = elem->value;

        prev->next = elem->next;
    }
          

    free(elem);
         

    sem_post(&m);
    return 0;
}
