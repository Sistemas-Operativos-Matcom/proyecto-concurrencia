#include <pthread.h>
#include <stdlib.h>

#ifndef INT_LL_H
#define INT_LL_H

typedef struct Node {
    int value;           
    struct Node* next;
} node;
typedef struct _integer_linked_list_t {
    node* first;
    int count;
    pthread_mutex_t lock;
} int_ll_t;

int init_list(int_ll_t *list);
int free_list(int_ll_t *list);
int size_list(int_ll_t *list);
int index_list(int_ll_t *list, int index, int *out_value);
int insert_list(int_ll_t *list, int index, int value);
int remove_list(int_ll_t *list, int index, int *out_value);
#endif
