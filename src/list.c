#include "list.h"
#include "stdlib.h"

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&list->lock, NULL);
    list->raiz = NULL;
    list->size = 0;

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);

    node_t* current = list->raiz;
    while(current != NULL)
    {
        node_t* tofree = current;
        current = current->next;
        free(tofree);
    }

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    
    int size = list->size;

    pthread_mutex_unlock(&list->lock);

    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);

    if(list->size == 0) 
    {
        pthread_mutex_unlock(&list->lock);
        
        return 1;
    }

    node_t* current = list->raiz;

    if(index > 0)
    {
        if(index >= list->size) { index = list->size - 1; }

        int count = 0;
        while(current != NULL)
        {
            if(count == index) { break; }

            current = current->next;
            count ++;
        }
    }

    *out_value = current->value;

    pthread_mutex_unlock(&list->lock);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    
    node_t* toinsert = (node_t*)malloc(sizeof(node_t));
    if(toinsert == NULL)
    {
        pthread_mutex_unlock(&list->lock);

        return 1;
    }

    if(list->size == 0) { list->raiz = toinsert; }
    else if(index <= 0)
    {
        toinsert->next = list->raiz;
        list->raiz = toinsert;
    } 
    else
    {
        node_t* current = list->raiz;
        int count = 0;

        while(current->next != NULL)
        {
            if(count == index - 1) { break; }

            current = current->next;
            count ++;
        }
        if(current->next == NULL) { current->next = toinsert; }
        else
        {
            toinsert->next = current->next;
            current->next = toinsert;
        }
    }
    toinsert->value = value;
    list->size ++;

    pthread_mutex_unlock(&list->lock);

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    
    if(list->size == 0) 
    {
        pthread_mutex_unlock(&list->lock);

        return 1;
    }
    
    node_t* toremove;

    if(index <= 0)
    {
        toremove = list->raiz;
        list->raiz = toremove->next;
    } 
    else
    {
        if(index >= list->size) { index = list->size - 1; }

        node_t* current = list->raiz;
        int count = 0;

        while(current->next != NULL)
        {
            if(count == index - 1) { break; }

            current = current->next;
            count ++;
        }
        if(current->next == NULL) { return 1; }
        else
        {
            toremove = current->next;
            current->next = toremove->next;
        }
    }

    *out_value = toremove->value;
    free(toremove);
    list->size --;

    pthread_mutex_unlock(&list->lock);

    return 0;
}
