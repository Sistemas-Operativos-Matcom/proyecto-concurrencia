#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


static int get_size(treap_node_t *node){
    return node ? node->size : 0;
}

static void update_size(treap_node_t *node){
    if (node){
        node->size = get_size(node->left) + 1 + get_size(node->right);
    }
}

static treap_node_t *create_node(int value){
    treap_node_t *node = (treap_node_t *)malloc(sizeof(treap_node_t));
    node->value = value;
    node->priority = rand(); 
    node->size = 1;
    node->left = node->right = NULL;
    return node;
}

static void split(treap_node_t *t, int key, treap_node_t **left, treap_node_t **right){
    if (!t){
        *left = *right = NULL;
    }
    else if (key <= get_size(t->left)){ 
        split(t->left, key, left, &t->left);
        *right = t;
    }
    else
    {
        split(t->right, key - get_size(t->left) - 1, &t->right, right);
        *left = t;
    }
    update_size(t);
}

static treap_node_t *merge(treap_node_t *left, treap_node_t *right){
    if (!left || !right)
        return left ? left : right;

    if (left->priority > right->priority){
        left->right = merge(left->right, right);
        update_size(left);
        return left;
    }else{
        right->left = merge(left, right->left);
        update_size(right);
        return right;
    }
}


void treap_init(treap_t *treap){
    pthread_mutex_init(&treap->mutex, NULL);
    treap->root = NULL;
}

void treap_insert(treap_t *treap, int value, int pos){
    pthread_mutex_lock(&treap->mutex);

    if (pos < 0 || pos > get_size(treap->root)){
        pthread_mutex_unlock(&treap->mutex);
        return;
    }

    treap_node_t *new_node = create_node(value);
    treap_node_t *left, *right;
    split(treap->root, pos, &left, &right);
    treap->root = merge(merge(left, new_node), right);

    pthread_mutex_unlock(&treap->mutex);
}

void treap_delete(treap_t *treap, int pos){
    pthread_mutex_lock(&treap->mutex);

    if (pos < 0 || pos >= get_size(treap->root)){ 
        pthread_mutex_unlock(&treap->mutex);
        return; 
    }

    treap_node_t *left, *mid, *right, *found;
    split(treap->root, pos, &left, &mid);
    split(mid, 1, &found, &right);

    if (found){
        free(found);
    }

    treap->root = merge(left, right);

    pthread_mutex_unlock(&treap->mutex);
}

int treap_find(treap_t *treap, int pos){
    pthread_mutex_lock(&treap->mutex);

    if (pos < 0 || pos >= get_size(treap->root)){
        pthread_mutex_unlock(&(treap->mutex));
        return -1;
    }

    treap_node_t *t = treap->root;
    int res = -1;
    while (t){
        int left_size = get_size(t->left);
        if (pos < left_size){
            t = t->left;
        }else if (pos > left_size){
            pos -= left_size + 1;
            t = t->right;
        }else{
            res = t->value;
            break;
        }
    }
    pthread_mutex_unlock(&treap->mutex);
    return res;
}

static void free_treap_recursive(treap_node_t *node){
    if (node != NULL){
        free_treap_recursive(node->left);
        free_treap_recursive(node->right);
        free(node);
    }
}

void treap_free(treap_t *treap){
    pthread_mutex_lock(&treap->mutex);
    free_treap_recursive(treap->root);
    pthread_mutex_unlock(&treap->mutex);
    pthread_mutex_destroy(&treap->mutex);
}

int treap_get_size(const treap_t *treap){
    pthread_mutex_lock(&((treap_t *)treap)->mutex);
    int size = get_size(treap->root);
    pthread_mutex_unlock(&((treap_t *)treap)->mutex);
    return size;
}

//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================

int init_list(int_ll_t *list){
    pthread_mutex_init(&(list->mutex), NULL); 
    treap_init(&list->treap);                 
    return 0;
}

// Free list structure
int free_list(int_ll_t *list){
    if (list == NULL)
        return -1;
    pthread_mutex_lock(&(list->mutex));
    treap_free(&list->treap); 
    pthread_mutex_unlock(&(list->mutex));
    pthread_mutex_destroy(&(list->mutex)); 
    return 0;
}

// Get list size
int size_list(int_ll_t *list){
    pthread_mutex_lock(&(list->mutex));
    int size = treap_get_size(&list->treap); 
    pthread_mutex_unlock(&(list->mutex));
    return size;
}

// Get element at index
int index_list(int_ll_t *list, int index, int *out_value){
    if (list == NULL || out_value == NULL)
        return -1;
    pthread_mutex_lock(&(list->mutex));
    int size = treap_get_size(&list->treap);

    if (size == 0){
        pthread_mutex_unlock(&(list->mutex));
        return -1; // Lista vacía
    }
    if (index < 0)
        index = 0;
    else if (index >= size)
        index = size - 1;

    *out_value = treap_find(&list->treap, index); 
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Insert element at index
int insert_list(int_ll_t *list, int index, int value){
    if (list == NULL)
        return -1;
    pthread_mutex_lock(&(list->mutex));
    int size = treap_get_size(&list->treap);

    if (index < 0)
        index = 0;
    else if (index > size)
        index = size;

    treap_insert(&list->treap, value, index); 
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}

// Remove element at index
int remove_list(int_ll_t *list, int index, int *out_value){
    if (list == NULL || out_value == NULL)
        return -1;
    pthread_mutex_lock(&(list->mutex));
    int size = treap_get_size(&list->treap);

    if (size == 0){
        pthread_mutex_unlock(&(list->mutex));
        return -1; // Lista vacía
    }
    if (index < 0)
        index = 0;
    else if (index >= size)
        index = size - 1;

    *out_value = treap_find(&list->treap, index); 
    treap_delete(&list->treap, index);            
    pthread_mutex_unlock(&(list->mutex));
    return 0;
}