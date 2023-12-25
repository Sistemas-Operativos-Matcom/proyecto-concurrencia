#include "list.h"
#include "malloc.h"

// Init list structure
int init_list(int_ll_t *list)
{
    list->start = malloc(32 * sizeof(int));
    list->count = 0;
    list->max_size = 32;
    return 0;
}

void Check_Size(int_ll_t *list)
{
    if (list->count >= list->max_size)
    {
        list = realloc(list->start, list->max_size * 2);
    }
}

// Free list structure
int free_list(int_ll_t *list)
{
    free(list->start);
    free(list);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    return list->count;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    *out_value = list->start[index];
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= list->count)
    {
        index = list->count - 1;
    }

    Check_Size(list);

    for (int i = list->count; i > index; i--)
    {
        list->start[i] = list->start[i - 1];
    }
    list->start[index] = value;
    list->count++;
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= list->count)
    {
        index = list->count - 1;
    }

    *out_value = list->start[index];
    for (int i = index; i < list->count; i++)
    {
        list->start[i] = list->start[i + 1];
    }
    list->count--;
    return 0;
}