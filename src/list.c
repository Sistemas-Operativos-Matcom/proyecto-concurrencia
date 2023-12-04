#include "list.h"

// Init list structure
int init_list(linkedlist_t *list){
    list->root = NULL;
    list->size = 0;

    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(linkedlist_t *list){
    pthread_mutex_lock(&list->lock);
    while (list->root != NULL)
    {
        node* temp = list->root;
        list->root = list->root->next;
        free(temp);
    }
    list->size = 0;

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(linkedlist_t *list){
    pthread_mutex_lock(&list->lock);
    int size = list->size;
    pthread_mutex_unlock(&list->lock);
    return size;
}

// Get element at index
int index_list(linkedlist_t *list, int index, int *out_value){
    pthread_mutex_lock(&list->lock);
    
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if (index < 0)
    {
        index = 0;
    }
    if (index >= list->size)
    {
        index = list->size-1;
    }

    int count = 0;
    node* temp = list->root;
    while (count < index && temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    if (temp != NULL) {
        *out_value = temp->value;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Insert element at index
int insert_list(linkedlist_t *list, int index, int value){
    pthread_mutex_lock(&list->lock);

    if (index < 0)
    {
        index = 0;
    }
    if (index > list->size)
    {
        index = list->size;
    }
    
    if (index == 0)
    {
        node* new = (node*)malloc(sizeof(node));
        new->value = value;

        new->next = list->root;
        list->root = new;
        list->size++;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }

    int count = 0;
    node* temp = list->root;
    while (count < index - 1)
    {
        count++;
        temp = temp->next;
    }

    node* new = (node*)malloc(sizeof(node));
    new->value = value;

    new->next = temp->next;
    temp->next = new;
    list->size++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(linkedlist_t *list, int index, int *out_value){
    pthread_mutex_lock(&list->lock);
    if (list->size == 0)
    {
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    
    if (index <= 0)
    {
        node* temp = list->root;
        *out_value = temp->value;
        list->root = list->root->next;
        free(temp);
        list->size--;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    
    if (index >= list->size)
    {
        index = list->size -1;
    }

    int count = 0;
    node* temp = list->root;
    while (count < index-1)
    {
        count++;
        temp = temp->next;
    }

    node* next = temp->next;
    *out_value = next->value;
    temp->next = temp->next->next;
    free(next);
    list->size--;
    pthread_mutex_unlock(&list->lock);
    return 0;
}
