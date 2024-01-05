#include "list.h"
#include <stdlib.h>
#include <pthread.h>

int init_list(int_ll_t *list) {
    list->count = 0;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

int free_list(int_ll_t *list) {
    pthread_mutex_lock(&list->lock);
    while (list->count > 0) {
        node* c = list->first;
        list->first = list->first->next;
        free(c);
        list->count = list->count - 1;
    }
    free(list);
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

int size_list(int_ll_t *list) {
    pthread_mutex_lock(&list->lock);
    int size = list->count;
    pthread_mutex_unlock(&list->lock);
    return size;
}

int index_list(int_ll_t *list, int index, int *out_value) {
    pthread_mutex_lock(&list->lock);
    if (index >= list->count) index = list->count - 1;
    if (index < 0) index = 0;
    node* current;
    if(list->count > 0)
    {
        current = list->first;
        int i = 0;
        while (i < index) {
            current = current->next;
            i++;
        }
        *out_value = current->value;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

int insert_list(int_ll_t *list, int index, int value) {
    pthread_mutex_lock(&list->lock);
    if (index >= list->count) index = list->count - 1;
    if (index < 0) index = 0;

    if(index == 0) {
        node* tmp = (node*)malloc(sizeof(node));
        tmp->value = value;
        tmp->next = list->first;
        list->first = tmp;
    }
    if(index > 0) {
        node* tmp = (node*)malloc(sizeof(node));
        tmp->value = value;
        node* current = list->first;
        int i = 0;
        while (i < index - 1) {
            current = current->next;
            i++;
        }
        tmp->next = current->next;
        current->next = tmp;
    } 
    list->count = list->count + 1;
    pthread_mutex_unlock(&list->lock);
    return 0;
}
// current tmp
int remove_list(int_ll_t *list, int index, int *out_value) {
    pthread_mutex_lock(&list->lock); 
    if(list->count == 0) {
       pthread_mutex_unlock(&list->lock);
        return 1; 
    }
    int current;
    if (index >= list->count) index = list->count - 1;
    if (index < 0) index = 0;
    if(index == 0) {
        current = list->first->value;
        list->first = list->first->next;
    }
    if(index > 0) {
        node* tmp = list->first;
        int i = 0;
        while (i < index - 1) {
            tmp = tmp->next;
            i++;
        }
        current = tmp->next->value;
        tmp->next = tmp->next->next;
    } 
    *out_value = current;
    list->count = list->count - 1;
    pthread_mutex_unlock(&list->lock);
    return 0;
}
