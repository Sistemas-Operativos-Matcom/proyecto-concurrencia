#include <stdio.h>
#include <stdlib.h>
#include "list.h"


// Init the list 
int init_list(int_ll_t *list)
{
    list->root = NULL;
    list->size = 0;
    return 0;
}

// Size of the List
int size_list(int_ll_t *list)
{
    return list->size;
}

// Free list structure
int free_list(int_ll_t *list)
{
    node_t *curr = list->root;
    node_t *next;

    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }

    list->root = NULL;
    list->size = 0;
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // Fixing index.
    index = (index < 0)? 0 : index;
    index = (index >= list->size)? list->size-1 : index;

    // Creating the new node to add.
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;

    // Adding the new node.
    node_t *curr = list->root;
    if (curr == NULL)
    {
        list->root = new_node;
    }
    else 
    {
        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
    list->size++;  
    return 0;
}


// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    // Fixing index.
    index = (index < 0)? 0 : index;
    index = (index >= list->size)? list->size-1 : index;

    // Finding the node.
    node_t *curr = list->root;

    if (curr == NULL)
    {
        return 1;
    }
    else 
    {
        node_t* prev = NULL;  
        for (int i = 0; i < index; i++)
        {
            prev = curr;
            curr = curr->next;
        }
        // Deleting the node and fixing references.
        *out_value = curr->value;
        if(prev != NULL)
            prev->next = curr->next;
        free(curr);
    }
    list->size--;  
    return 0;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // Fixing index.
    index = (index < 0)? 0 : index;
    index = (index >= list->size)? list->size-1 : index;

    node_t *curr = list->root;
    if(curr == NULL)
        return 1;
    
    // Finding the node.
    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }

    *out_value = curr->value;
    return 0;
}


// int main()
// {
//     int_ll_t* list = malloc(sizeof(int_ll_t));
//     init_list(list);

//     for (int i = 0; i < 10; i++)
//     {
//         insert_list(list,i,i);
//     }

//     int arr[10]; 
//     int val = -1;
//     for (int i = 0; i < 10; i++)
//     {
//         index_list(list,i,&val);
//         arr[i] = val;

//         // printf("val at %d ", i);
//         // printf("%d ", val); 
//     }
    
//     remove_list(list, 4 , &val);
//     remove_list(list, 8 , &val);

//     insert_list(list, 3 , 4);
//     insert_list(list, 8 , 9);

    
//     return 0;
// }