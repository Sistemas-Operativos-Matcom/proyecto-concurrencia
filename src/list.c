#include "list.h"

// Init list structure
int init_list(int_ll_t *list)
{
    // Inicializar atributos de la lista
    list->root = NULL;
    list->size = 0;

    // Inicializar lock
    pthread_mutex_init(&list->lock, NULL);
    return 0;
}

// Free list structure
int free_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    // Eliminar y limpiar cada nodo de la lista
    while (list->root != NULL)
    {
        node* temp = list->root;
        list->root = list->root->next;
        free(temp);
    }
    list->size = 0;

    // Destruir los locks
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
    return 0;
}

// Get list size
int size_list(int_ll_t *list)
{
    pthread_mutex_lock(&list->lock);
    int size = list->size;
    pthread_mutex_unlock(&list->lock);
    return size;
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
