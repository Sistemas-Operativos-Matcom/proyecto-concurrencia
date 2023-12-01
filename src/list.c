#include <stdio.h>

#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list = malloc(sizeof(int_ll_t));
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    list->size = 0;
    list->nodes = NULL;
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
    ll_node_t *current = list->nodes;

    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        current = current->next;
    }

    if(current == NULL) return 1;

    *out_value = current->value;

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    ll_node_t *current = list->nodes;

    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        current = current->next;
    }

    ll_node_t *new = malloc(sizeof(ll_node_t));
    new->value = value;

    if(current == NULL) {
        current = new;
        return 0;
    }

    if(current->next == NULL) {
        current->next = new;
    } else {
        ll_node_t *next = current->next;
        new->next = next;
        current->next = new;
    }

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    ll_node_t *current = list->nodes;
    ll_node_t *prev = NULL;

    for(int i = 0; i < index; i++) {
        if(current->next == NULL) break;

        prev = current;
        current = current->next;
    }

    if(current == NULL) return 1;

    if(prev == NULL) {
        list->nodes = current->next;
    } else {
        prev->next = current->next;
    }

    *out_value = current->value;

    return 0;
}
