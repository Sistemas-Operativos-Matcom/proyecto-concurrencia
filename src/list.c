#include "list.h"
#include <stdlib.h>

// Init list structure
int init_list(int_ll_t *list)
{
    pthread_mutex_init(&(list->mutex), NULL);  // Initialize the mutex for thread safety
    // Initialize the linked list structure
    list->head = NULL;              // Set the head pointer to NULL (empty list)
    list->size = 0;                 // Initialize size to zero (empty list)
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    if (list == NULL) return -1;    // Check if list is NULL
    // Free the allocated memory for each node and destroy the mutex
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);                 // Free memory of each node
    }

    pthread_mutex_destroy(&(list->mutex));  // Destroy the mutex
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&(list->mutex));   
    int size = list->size;                
    pthread_mutex_unlock(&(list->mutex)); 
    return size;                          
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));   // Lock the mutex for thread safety

    if (list == NULL)
    {
        pthread_mutex_unlock(&(list->mutex));  
        return 1;  // Check for invalid parameters
    }    

    if(index < 0) index = 0;             // Set index to zero if negative
    else if (index >= list->size) index = list->size - 1;  // Set index to the last node if out of range
    
    Node *current = list->head;
    int i;
    for (i = 0; i < index && current != NULL; i++) {
        current = current->next;          // Traverse to the specified index
    }

    if (current != NULL) {
        *out_value = current->value;      // Get the value at the specified index
    } else if (list->size > 0) {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;      // Traverse to the last node if index is out of range
        }
        *out_value = current->value;      // Get the value of the last node
    } else {
        pthread_mutex_unlock(&(list->mutex));  // Unlock the mutex before returning error for an empty list
        return -1;                           // Empty list error
    }

    pthread_mutex_unlock(&(list->mutex));   // Unlock the mutex
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    pthread_mutex_lock(&(list->mutex));   // Lock the mutex for thread safety

    if (list == NULL) 
    {
        pthread_mutex_unlock(&(list->mutex));
        return -1;  // Check for invalid parameters
    }
    if (index < 0) index = 0;             // Set index to zero if negative
    else if (index > list->size) index = list->size; // Set index to the last node if out of range
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) 
    {
        pthread_mutex_unlock(&(list->mutex));
        return -1;          // Check for memory allocation error
    }
    new_node->value = value;
    new_node->next = NULL;


    if (!index) {
        new_node->next = list->head;     // Insert at the beginning
        list->head = new_node;
    } else {
        Node *current = list->head;
        int i;
        for (i = 0; i < index - 1 && current != NULL; i++) {
            current = current->next;      // Traverse to the node before the specified index
        }

        if (current != NULL) {
            new_node->next = current->next; // Insert at the specified index
            current->next = new_node;
        } else {
            free(new_node);               // Free memory if index is out of range
            pthread_mutex_unlock(&(list->mutex));  // Unlock the mutex before returning error
            return -1;                  // Index out of range error
        }
    }

    list->size++;                      // Increment the size of the list
    pthread_mutex_unlock(&(list->mutex));  // Unlock the mutex
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    pthread_mutex_lock(&(list->mutex));   // Lock the mutex for thread safety

    if (list == NULL || list->size == 0) 
    {
        pthread_mutex_unlock(&(list->mutex));
        return 1;  // Check for invalid parameters or empty list
    }
    if (index < 0) index = 0;             // Set index to zero if negative
    else if (index >= list->size) index = list->size - 1; // Set index to the last node if out of range
    Node *temp = NULL;
    if (!index) {
        temp = list->head;
        *out_value = temp->value;         // Get the value of the node to be removed
        list->head = temp->next;          // Remove from the beginning
    } else {
        Node *current = list->head;
        int i;
        for (i = 0; i < index - 1 && current != NULL; i++) {
            current = current->next;      // Traverse to the node before the specified index
        }

        if (current != NULL && current->next != NULL) {
            temp = current->next;
            *out_value = temp->value;     // Get the value of the node to be removed
            current->next = temp->next;   // Remove at the specified index
        } else {
            pthread_mutex_unlock(&(list->mutex));  // Unlock the mutex before returning error
            return -1;                  // Index out of range error
        }
    }

    free(temp);                       // Free memory of the removed node
    list->size--;                     // Decrement the size of the list
    pthread_mutex_unlock(&(list->mutex));  // Unlock the mutex
    return 0;
}
