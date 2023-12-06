#include "list.h"
#include "stdlib.h"

// Initialize the list structure
int init_list(int_ll_t *list){
    pthread_mutex_init(&list->lock, NULL); // Initialize the lock
    list->head = NULL;
    list->size = 0;
    return 0;
}

// Free the list structure
int free_list(int_ll_t *list){
    if(list == NULL) return -1; // Return -1 to indicate an error

    // Traverse the list freeing all it's nodes
    node_t* cur = list->head;
    while(cur != NULL){
        node_t* t = cur;
        cur = cur->next;
        free(t);
    }

    pthread_mutex_destroy(&list->lock); // Delete the lock
    return 0;
}

// Get the size of the list
int size_list(int_ll_t *list){
    pthread_mutex_lock(&list->lock); // Acquire the lock to ensure thread safety
    int size = list->size;
    pthread_mutex_unlock(&list->lock); // Release the lock
    return size;
}

// Get the value at the specified index
int index_list(int_ll_t *list, int index, int *out_value){
    pthread_mutex_lock(&list->lock); // Acquire the lock to ensure thread safety

    // Check if the list is null
    if(list == NULL){
        pthread_mutex_unlock(&list->lock); // Release the lock
        return -1; // Return -1 to indicate an error
    }

    // Ensure the index is within the valid range
    index = (index < 0) ? 0 : (index >= list->size) ? list->size - 1 : index;

    // Traverse the list to find the node at index
    node_t* cur = list->head;
    int i = 0;
    for(; i < index && cur != NULL; i++, cur = cur->next);

    if(cur != NULL) {
        *out_value = cur->value; // Set the output value to the value at the specified index
    } 
    else if(list->size > 0) {
        cur = list->head;
        while(cur->next != NULL){
            cur = cur->next;
        }
        *out_value = cur->value;
    } 
    else {
        pthread_mutex_unlock(&list->lock); // Release the lock
        return -1; // Return -1 to indicate an error
    }

    pthread_mutex_unlock(&list->lock); // Release the lock
    return 0; // Return 0 to indicate success
}

// Insert an element at the specified index
int insert_list(int_ll_t *list, int index, int value) {
    // Check if the list is null
    if (list == NULL) {
        return -1; // Return -1 to indicate an error
    }

    pthread_mutex_lock(&list->lock); // Acquire the lock to ensure thread safety

    // Ensure the index is within the valid range
    index = (index < 0) ? 0 : (index > list->size) ? list->size : index;

    // Memory allocation for the new node
    node_t* x = (node_t*)malloc(sizeof(node_t));
    if (x == NULL) {
        pthread_mutex_unlock(&list->lock); // Release the lock
        return -1; // Return -1 to indicate an error
    }
    x->value = value;
    x->next = NULL;

    if (index == 0) {
        // Insert at the beginning
        x->next = list->head;
        list->head = x;
    } 
    else {
        // Insert at the specified index
        node_t* predecessor = list->head;
        // Traverse the list to find the node at index-1
        int i = 0;
        for (; i < index - 1 && predecessor != NULL; i++, predecessor = predecessor->next);
        if (predecessor != NULL) {
            x->next = predecessor->next;
            predecessor->next = x;
        } else {
            free(x); // free the allocated memory
            pthread_mutex_unlock(&list->lock); // Release the lock
            return -1; // Return -1 to indicate an error
        }
    }

    list->size++; // Update the list size

    pthread_mutex_unlock(&list->lock); // Release the lock
    return 0; // Return 0 to indicate success
}

// Remove the element at the specified index
int remove_list(int_ll_t *list, int index, int *out_value) {
    // Check if the list is null or empty
    if (list == NULL || list->size == 0) {
        return -1; // Return -1 to indicate an error
    }

    pthread_mutex_lock(&list->lock); // Acquire the lock to ensure thread safety

    // Ensure the index is within the valid range
    index = (index < 0) ? 0 : (index >= list->size) ? list->size - 1 : index;

    node_t *x = NULL;
    if (index == 0) {
        // If the index is at the beginning, update the head pointer
        x = list->head;
        list->head = x->next;
    } 
    else {
        // Traverse the list to find the node at index-1
        node_t *cur = list->head;
        int i = 0;
        for (; i < index - 1 && cur->next != NULL; i++, cur = cur->next);
        if (cur->next != NULL) {
            // Remove the node at the specified index by updating pointers
            x = cur->next;
            cur->next = x->next;
        } 
        else {
            // If the index is out of range, release the lock and return error
            pthread_mutex_unlock(&list->lock);
            return -1;
        }
    }

    *out_value = x->value; // Store the value of the removed node
    free(x); // Free the memory of the removed node
    list->size--; // Update the list size

    pthread_mutex_unlock(&list->lock); // Release the lock
    return 0; // Return 0 to indicate success
}
