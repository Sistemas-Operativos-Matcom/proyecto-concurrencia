#include "list.h"
#include <stdlib.h>

int init_list(int_ll_t *list) {
    if (list == NULL) return -1;
    list->head = NULL;  // Initialize the head pointer to NULL
    return 0;
}

int free_list(int_ll_t *list) {
    if (list == NULL) return -1;

    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    
    list->head = NULL;  // Set head to NULL after freeing all nodes
    return 0;
}

int size_list(int_ll_t *list) {
    if (list == NULL) return -1;

    int size = 0;
    Node *current = list->head;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

int index_list(int_ll_t *list, int index, int *out_value) {
    if (list == NULL || index < 0) return -1;

    Node *current = list->head;
    int i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        *out_value = current->value;
        return 0;
    }

    return -1;  // Index out of range or empty list
}

int insert_list(int_ll_t *list, int index, int value) {
    if (list == NULL || index < 0) return -1;

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) return -1;
    
    new_node->value = value;

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
        return 0;
    }

    Node *current = list->head;
    int i = 0;
    while (current != NULL && i < index - 1) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        new_node->next = current->next;
        current->next = new_node;
        return 0;
    }

    free(new_node);
    return -1;  // Index out of range or empty list
}

int remove_list(int_ll_t *list, int index, int *out_value) {
    if (list == NULL || index < 0 || list->head == NULL) return -1;

    if (index == 0) {
        Node *temp = list->head;
        *out_value = temp->value;
        list->head = list->head->next;
        free(temp);
        return 0;
    }

    Node *current = list->head;
    int i = 0;
    while (current != NULL && i < index - 1) {
        current = current->next;
        i++;
    }

    if (current != NULL && current->next != NULL) {
        Node *temp = current->next;
        *out_value = temp->value;
        current->next = temp->next;
        free(temp);
        return 0;
    }

    return -1;  // Index out of range or empty list
}
