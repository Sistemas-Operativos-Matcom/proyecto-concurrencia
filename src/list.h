#ifndef INT_LL_H
#define INT_LL_H
#include <pthread.h>


typedef struct treap_node{
    int value;
    int key,priority;
    struct treap_node *left,*right;
    int size; 
} treap_node_t;

typedef struct{
    treap_node_t *root;
    pthread_mutex_t mutex;
} treap_t;

void treap_init(treap_t *treap);
void treap_insert(treap_t *treap,int value,int pos);
void treap_delete(treap_t *treap,int pos);
int treap_find(treap_t *treap,int pos);
void treap_print(const treap_t *treap);
void treap_free(treap_t *treap);
int treap_get_size(const treap_t *treap);




// Integer Linked Lists
typedef struct _integer_linked_list_t{
    treap_t treap;
    pthread_mutex_t mutex;
} int_ll_t;

// Init list structure
int init_list(int_ll_t *list);

// Free list structure
int free_list(int_ll_t *list);

// Get list size
int size_list(int_ll_t *list);

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value);

// Insert element at index
int insert_list(int_ll_t *list, int index, int value);

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value);
#endif
