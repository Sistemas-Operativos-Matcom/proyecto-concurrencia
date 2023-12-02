#include "list.h"
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h>

sem_t mutex;                  

// Init list structure
int init_list(int_ll_t *list)
{
    sem_init(&mutex,0,1);
    list=malloc(sizeof(list));
    list->Ini=NULL;
    list->sz=0;
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    sem_wait(&mutex);
    while(list->sz){
        if(list->sz>1){
            node *f=(list->Ini)->next;
            free(list->Ini);
            list->Ini=f;
        }else{
            free(list->Ini);
        }
        list->sz-=1;
    }
    free(list);
    sem_post(&mutex);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    sem_wait(&mutex);
    int d=list->sz;
    sem_post(&mutex);
    return d;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    sem_wait(&mutex);
    if(index<0)index=0;
    if(index>=list->sz)index=(list->sz)-1;
    if(index>=0){
        node* cur=list->Ini;
        while(index){
            cur=cur->next;
            index--;
        }
        *out_value=cur->data;
    }
    sem_post(&mutex);
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    sem_wait(&mutex);
    if(index<0)index=0;
    if(index>=list->sz)index=(list->sz);
   if(index){
    int ind=0;
    node *cur=list->Ini;
    while(index-ind-1){
        cur=cur->next;
        ind++;
    } 
    node *N=malloc(sizeof(node));
    N->next=NULL;
    N->prev=NULL;
    N->data=value;
    N->prev=cur;
    if(list->sz-index){
    N->next=cur->next;
    (N->next)->prev=N;
    }
    cur->next=N;
   }else{
    node *N=malloc(sizeof(node));
    N->next=NULL;
    N->prev=NULL;
    N->data=value;
    if(list->sz-index){
    N->next=list->Ini;
    (list->Ini)->prev=N;
    }
    list->Ini=N;
   }
   list->sz++;
    sem_post(&mutex);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    sem_wait(&mutex);
    if(index<0)index=0;
    if(index>=list->sz)index=(list->sz)-1;
    int ind=0;
    node *cur=list->Ini;
    while(index-ind){
        cur=cur->next;
        ind++;
    } 
    *out_value=cur->data;
    if(index){
        (cur->prev)->next=(cur->next);
    }else{
        list->Ini=cur->next;
    }
    if(list->sz-index-1){
        (cur->next)->prev=(cur->prev);
    }
    free(cur); 
   list->sz--;
    sem_post(&mutex);
    return 0;
}
