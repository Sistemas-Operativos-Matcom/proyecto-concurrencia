#include "list.h"

int bound_index(int index, int size);
node *find_node(int_ll_t *list, int index);

// Init list structure
int init_list(int_ll_t *list)
{
    list->head = NULL;
    list->size = 0;
    return pthread_mutex_init(&list->lock, NULL);
}

// Free list structure
int free_list(int_ll_t *list)
{
    //// Starts Critical Section
    pthread_mutex_lock(&list->lock);
    ////

    node *_node = list->head;

    while (_node != NULL)
    {
        node *temp = _node;
        _node = _node->next;
        free(temp);
    }

    pthread_mutex_t lock = list->lock;
    free(list);

    ////
    pthread_mutex_destroy(&lock); // destroying lock for good
    //// Ends Critical Section

    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    int size;

    //// Starts Critical Section
    pthread_mutex_lock(&list->lock);
    ////

    size = list->size;

    ////
    pthread_mutex_unlock(&list->lock);
    //// Ends Critical Section

    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    //// Starts Critical Section
    pthread_mutex_lock(&list->lock);
    ////

    int size = list->size;
    index = bound_index(index, size);

    // Handling possible exception (empty list)
    if (size == 0)
    {
        perror("Invalid index, List is empty");
        pthread_mutex_unlock(&list->lock);
        return -1;
    }

    *out_value = find_node(list, index)->value;

    ////
    pthread_mutex_unlock(&list->lock);
    //// Ends Critical Section

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    //// Starts Critical Section
    pthread_mutex_lock(&list->lock);
    ////

    int size = list->size;
    index = index >= size ? size : bound_index(index, size);

    node *_node = (node *)malloc(sizeof(struct _node));

    // Handling possible malloc exception
    if (_node == NULL)
    {
        perror("Malloc error");
        pthread_mutex_unlock(&list->lock);
        return -1;
    }

    _node->value = value;
    if (index == 0)
    {
        _node->next = list->head;
        list->head = _node;
    }
    else
    {
        node *previous = find_node(list, index - 1);
        node *temp = previous->next;
        previous->next = _node;
        _node->next = temp;
    }

    list->size++;

    ////
    pthread_mutex_unlock(&list->lock);
    //// Ends Critical Section

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    //// Starts Critical Section
    pthread_mutex_lock(&list->lock);
    ////

    int size = list->size;
    index = bound_index(index, size);

    // Handling possible exception (empty list)
    if (size == 0)
    {
        perror("Cannot remove, List is empty");
        pthread_mutex_unlock(&list->lock);
        return -1;
    }

    node *_node;
    if (index == 0)
    {
        _node = list->head;
        list->head = _node->next;
    }
    else
    {
        node *previous = find_node(list, index - 1);
        _node = previous->next;
        previous->next = _node->next;
    }

    *out_value = _node->value;
    list->size--;
    free(_node);

    ////
    pthread_mutex_unlock(&list->lock);
    //// Ends Critical Section

    return 0;
}

// Private functions
int bound_index(int index, int size)
{
    if (size == 0 || index < 0)
        index = 0;
    else if (index > size - 1)
        index = size - 1;
    return index;
}

node *find_node(int_ll_t *list, int index)
{
    if (index < 0 || index > list->size - 1)
        fprintf(stderr, "Index outside of the bounds of the linked list"), exit(-1);

    if (index == 0)
        return list->head;

    node *current = list->head;
    int i = 0;
    while (i < index)
        current = current->next, i++;
    return current;
}