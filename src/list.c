#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list->f_node = NULL;
    list->lenght = 0;
    pthread_mutex_init(&list->lock, NULL);

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    free_nodes(list->f_node);
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    free(list);
    
    return 0;
}

void free_nodes(ll_node *node){
    if(node->n_node != NULL){
        free_nodes(node->n_node);
    }
    free(node);
}

// Get list size
int size_list(int_ll_t *list)
{
    
    return list->lenght;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    ll_node *r_node = list->f_node;

    if(index < 0){
        index = 0;
    };
    if(index >= list->lenght){
        index = list->lenght - 1;
    }

    pthread_mutex_lock(&list->lock);
    if(list->lenght == 0){
        pthread_mutex_unlock(&list->lock);
        return 1;
    }
    for(int i = 0; i < index; i++){
        r_node = r_node->n_node;
    }

    *out_value = r_node->value;
    pthread_mutex_unlock(&list->lock);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    if(index <= 0){ 
        index = 0;
    }
    if(index >= list->lenght){
        index = list->lenght - 1;
    }

    pthread_mutex_lock(&list->lock);
    if(index == 0){
        ll_node *new_node = malloc(sizeof(ll_node));
        new_node->n_node = list->f_node;
        new_node->value = value;
        list->f_node = new_node;
        list->lenght++;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }

    ll_node *this_node = list->f_node;
    for(int i = 0; i <= index - 1; i++){
        this_node = this_node->n_node;
    }

    ll_node *new_node = (ll_node*)malloc(sizeof(ll_node));
    new_node->n_node = this_node->n_node;
    new_node->value = value;

    this_node->n_node = new_node;
    list->lenght++;
    pthread_mutex_unlock(&list->lock);
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    if(index <= 0){ 
        index = 0;
    }
    if(index >= list->lenght){
        index = list->lenght - 1;
    }

    pthread_mutex_lock(&list->lock);

    if(list->lenght == 0){
        pthread_mutex_unlock(&list->lock);
        return 1;
    }

    if(index == 0){
        ll_node *this_node = list->f_node;
        *out_value = this_node->value;
        list->f_node = this_node->n_node;
        free(this_node);
        list->lenght--;
        pthread_mutex_unlock(&list->lock);
        return 0;
    }
    
    ll_node *this_node = list->f_node;
    for(int i = 0; i < index - 1; i++){
        this_node = this_node->n_node;
    }

    ll_node *remove_node = this_node->n_node;
    *out_value = remove_node->value;

    if(remove_node->n_node == NULL){
        free(remove_node);
        this_node->n_node = NULL;
    }
    else{
        this_node->n_node = remove_node->n_node;
        free(remove_node);
    }
    
    list->lenght--;
    pthread_mutex_unlock(&list->lock);
    return 0;
}
