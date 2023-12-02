#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Init list structure
node_t* root_node = NULL;

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
   node_t* curr = list->root;
   node_t* next;

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
   node_t* new_node = (node_t*)malloc(sizeof(node_t));
   new_node->value = value; 
   new_node->next = NULL;

   node_t* curr = list->root; 
   node_t* prev = NULL;

   if(index == 0)
   {
       new_node->next = curr;
       list->root = new_node;
   }
   else
   {
       for (size_t i = 0; i < index; i++)
       {
           prev = curr;
           curr = curr->next;
       }

       new_node->next = curr;
       prev->next = new_node;
   }

   list->size++;
   return 0; 
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
   node_t* curr = list->root; 
   node_t* prev = NULL;

   for (size_t i = 0; i < index; i++)
   {
       prev = curr;
       curr = curr->next;
   }

   prev->next = curr->next;
   *out_value = curr->value;

   free(curr);
   list->size--;
   return 0; 
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
   index = (index >= list->size)? list->size-1 : index;
   index = (index < 0)? 0 : index; 

   if(list->size == 0)
       return 1;

   node_t* curr = list->root; 
   for (size_t i = 0; i < index; i++)
   {
       curr = curr->next;
   }

   *out_value = curr->value;
   return 0;
}