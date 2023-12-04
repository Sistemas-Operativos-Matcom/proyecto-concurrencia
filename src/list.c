#include "list.h"

// Helper function to create a new node
node *create_node(int value)
{
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node != NULL)
    {
        atomic_init(&new_node->data, value);
        atomic_init(&new_node->next, 0);
        atomic_flag_clear(&new_node->deleted);
        // atomic_init(&new_node->deleted, 0);
    }
    return new_node;
}
// Init list structure
int init_list(int_ll_t *list)
{
    // atomic_store(&list->head, 0);
    // atomic_store(&list->sz, 0)
    node *dummy_node = create_node(0);
    if (dummy_node == NULL)
    {
        return -1; // Memory allocation failure
    }

    atomic_init(&list->head, (uintptr_t)dummy_node);
    atomic_init(&list->sz, 0);

    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    uintptr_t curr = atomic_load(&list->head);
    while (curr != 0)
    {
        node *curr_node = (node *)curr;
        uintptr_t next = atomic_load(&curr_node->next);
        if (!atomic_flag_test_and_set(&curr_node->deleted))
        {
            free(curr_node);
        }
        else
        {
            break;
        }
        curr = next;
    }
    atomic_store(&list->head, 0);
    atomic_store(&list->sz, 0);

    free(list);

    return 0;
}
void validate_index(int *index, int_ll_t *list)
{
    if (*index < 0)
        *index = 0;

    else if (*index >= atomic_load(&list->sz))
        *index = atomic_load(&list->sz);
}

// Get list size
int size_list(int_ll_t *list)
{
    return atomic_load(&list->sz);
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value)
{
    validate_index(&index, list);

    uintptr_t curr = atomic_load(&list->head);
    for (int i = 0; i < index; i++)
    {
        node *curr_node = (node *)curr;
        curr = atomic_load(&curr_node->next);
    }

    node *curr_node = (node *)curr;
    *out_value = atomic_load(&curr_node->data);

    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value)
{
    validate_index(&index, list);

    node *new_node = create_node(value);

    uintptr_t curr = atomic_load(&list->head);
    uintptr_t prev = 0;
    for (int i = 0; i < index; i++)
    {
        node *curr_node = (node *)curr;
        prev = curr;
        curr = atomic_load(&curr_node->next);
    }

    atomic_store(&new_node->next, curr);

    // Change the head of the list
    if (atomic_load(&list->head) == curr)
    {
        atomic_store(&list->head, (uintptr_t)new_node);
    }
    else
    {
        node *prev_node = (node *)prev;
        atomic_store(&prev_node->next, (uintptr_t)new_node);
    }

    atomic_fetch_add(&list->sz, 1);

    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value)
{
    validate_index(&index, list);

    uintptr_t curr = atomic_load(&list->head);
    uintptr_t prev = 0;
    for (int i = 0; i < index; i++)
    {
        node *curr_node = (node *)curr;
        prev = curr;
        curr = atomic_load(&curr_node->next);
    }
    node *curr_node = (node *)curr;

    if (prev == 0)
    {
        atomic_store(&list->head, atomic_load(&curr_node->next));
    }
    else
    {
        node *prev_node = (node *)prev;
        atomic_store(&prev_node->next, atomic_load(&curr_node->next));
    }

    *out_value = atomic_load(&curr_node->data);
    if (!atomic_flag_test_and_set(&curr_node->deleted))
    {
        free(curr_node);
    }
    atomic_fetch_sub(&list->sz, 1);

    return 0;
}
