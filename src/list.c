#include "list.h"
#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"

int size = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int_ll_t *add_element(int value, int_ll_t *next)
{
    int_ll_t *new = (int_ll_t *)malloc(sizeof(int_ll_t));
    new->value = value;
    new->next = next;
    pthread_mutexattr_init(&mutex);
    return new;
}

// Init list structure
int init_list(int_ll_t *list)
{
    // TODO: Your code here!
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    // TODO: Your code here!
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    // TODO: Your code here!
    return 0;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    // TODO: Your code here!
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    // TODO: Your code here!
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    // TODO: Your code here!
    return 0;
}
