#include "list.h"
#include "stdio.h"
#include "stdlib.h"
 
// Init list structure
int init_list(int_ll_t *list)
{
    list->root = NULL;
    list->size = 0;
    list->last = NULL;
    pthread_mutex_init(&list->lock, NULL);
    return 0;
} 

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    Node *u = list->root;
    while (u != NULL)
    {
        Node *v = u;
        u = u->next;
        free(v);
    }
    pthread_mutex_unlock(&list->lock);
    free(list);
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
    if(index <= 0)
    {
        *out_value = list->root->value;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    if(index >= list->size - 1)
    {
        *out_value = list->last->value;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    Node *u = list->root;
    while (u->index == index + 1)
    {
        if(u->index == index)
        {
            *out_value = u->value;
            pthread_mutex_unlock(&list->lock);
            return 0;
        }
        u = u->next;        
    }
    pthread_mutex_unlock(&list->lock);  
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&list->lock);
    //Node *node = newNode(index, value);
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) 
    {
        perror("malloc");
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    node->value = value;
    node->index = index;
    node->next = NULL;

    if(list->size == 0)
    {
        node->index = 0;
        list->root = node;
        list->last = node;
        list->size = 1;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    if(index <= 0)
    {
        node->next = list->root;
        node->index = 0;
        list->size += 1;
        list->root = node;

        Node *u = node->next;
        while (u != NULL)
        {
            u->index += 1;
            u = u->next;        
        }
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    if(index >= list->size - 1)
    {
        node->index = list->size;
        list->last->next = node;
        list->last = node;
        list->size += 1;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }    
    Node *u = list->root;
    int founded = 0;
    while (u != NULL)
    {
        if(founded) u->index += 1;
        if(u->index == node->index - 1)
        {
            founded = 1;
            Node *v = u->next;
            u->next = node;
            node->next = v;
            u = node->next;//saltarse "node" en el ciclo para no cambiar su indice
            continue;
        }
        u = u->next;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&list->lock);
    if(list->size == 0) return 0;
    if(index <= 0)
    {
        Node *node = list->root;
        *out_value = list->root->value;
        list->root = list->root->next;
        list->size -= 1;
        free(node);

        Node *u = list->root;
        while (u != NULL)
        {
            u->index -= 1;
            u = u->next;
        }
        pthread_mutex_unlock(&list->lock);
        return 0;       
    }
    if(index >= list->size - 1)
    {
        *out_value = list->last->value;
        list->size -= 1;
        Node *node = list->last;
        Node *u = list->root;
        while (u != NULL)
        {
            if(u->index == list->size - 1)
            {
                u->next = NULL;
                list->last = u;                
            }
            u = u->next;
        }
        free(node);
        pthread_mutex_unlock(&list->lock);
        return 0;        
    }

    Node *u = list->root;
    int founded = 0;
    while (u != NULL)
    {
        if(founded) u->index -= 1;
        if(u->index == index - 1)
        {
            founded = 1;
            Node *node = u->next;
            u->next = node->next;
            free(node);
            list->size -= 1;
        }
        u = u->next;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}
